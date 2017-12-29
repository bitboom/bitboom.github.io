---
layout: post
title: Recursive template를 이용한 tuple struct 제작
tags: [c++] 
---

C++에서는 서로 다른 타입의 데이터를 저장하기위해 std::tuple을 제공한다.
std::tuple은 그 자체로써 유용하지만 제네릭 프로그래밍을 공부하기에도 중요한 역할을 한다.  

실제 std::tuple과 같이 서로 다른 타입의 데이터를 저장하기 위한 구조체를 만들기 위해서는 
재귀 템플릿(recursive template)과 가변 인자 템플릿(variadic template)을 알고 있어야한다. 

#### keyword : template, recursive template, variadic template, [Parameter pack][1]

### 서로 다른 타입의 데이터를 저장할 수 있는 구조체 구현하기
```cpp
// g++ main.cpp -std=c++11
#include <iostream>

template<typename... Unpaked>
struct Pack {
	Pack() { std::cout << __PRETTY_FUNCTION__ << '\n'; }
};

template<typename Front, typename... Rest>
struct Pack<Front, Rest...> : public Pack<Rest...> {
	using Type = Front;
	using Base = Pack<Rest...>;

	Pack(Front front, Rest... rest): Base(rest...), value(front) {
		std::cout << __PRETTY_FUNCTION__ << '\n';
	}

	Type value;
};

int main() {
	Pack<> p0();
	Pack<int> p1(2);
	Pack<int, char> p2(1, 'c');

	struct Data {
	};

	Pack<int, char, Data> p3(1, 'c', Data());

	return 0;
}
```
C++ 문법상으로 보면 위의 예제는 재귀 템플릿이 호출 될 때마다 파라미터 팩이 계속적으로 복사가 되게 된다. 허나 아래와 같이 Perfect forwarding이 가능한 Helper Class를 만들면 lvalue에 대해서 최초 한번의 생성자만 호출되게 되고 오히려 rvalu를 넣었을 땐 컴파일러의 최적화를 방해하게 된다.

```cpp
template<typename... Args>
Pack<Args...> make_pack(Args&&... args) {
	return Pack<Args...>(std::forward<Args>(args)...);
}
```

### 컴파일러 최적화 확인 및 성능 측정 가이드 Snippet
아래 3경우를 직접 출력하여서 최적의 코드를 확인하여 보면 된다.

```cpp
// g++ main.cpp -std=c++11
#include <iostream>
#include <utility>

#define __LOGGING__ std::cout << __PRETTY_FUNCTION__ << '\n';

template<typename... Unpaked>
struct Pack {
	Pack() { __LOGGING__ } 
};

template<typename Front, typename... Rest>
struct Pack<Front, Rest...> : public Pack<Rest...> {
	using Type = Front;
	using Base = Pack<Rest...>;

	Pack(Front front, Rest... rest) : Base(rest...), value(front) { __LOGGING__ }

	Type value;
};

template<typename... Args>
Pack<Args...> make_pack(Args&&... args) {
	return Pack<Args...>(std::forward<Args>(args)...);
}

struct Noisy
{
	Noisy() { __LOGGING__ }
	Noisy(const Noisy&) { __LOGGING__ }
	Noisy(Noisy&&) { __LOGGING__ }
	~Noisy() {}

	Noisy& operator=(const Noisy&) { __LOGGING__ }
	Noisy&& operator=(Noisy&&) { __LOGGING__ }
};

int main() {
// Best solution
	Noisy n0, n1, n2;
	auto rvo_pack = make_pack(n0, n1, n2); // It is better solution with compiler

/*
	auto rvalue_pack = make_pack(Noisy(), Noisy(), Noisy());
*/

/*
	Noisy n0, n1, n2;
	Pack<Noisy, Noisy, Noisy> pack(n0, n1, n2);
*/

	return 0;
}
```
[1]: http://en.cppreference.com/w/cpp/language/parameter_pack
