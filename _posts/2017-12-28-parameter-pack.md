---
layout: post
title:  [WIP]Parameter Pack
tags: [c++] 
---

[WIP]
#### keyword : template, recursive template, variadic template
C++에서는 서로 다른 타입의 데이터를 저장하기위해 std::tuple을 제공한다.
std::tuple은 그 자체로써 유용하지만 제네릭 프로그래밍을 공부하기에도 중요한 역할을 한다.  

실제 std::tuple과 같이 서로 다른 타입의 데이터를 저장하기 위한 구조체를 만들기 위해서는 
재귀 템플릿(recursive template)과 가변 인자 템플릿(variadic template)을 알고 있어야한다. 

이는 STL에 [Parameter pack][1]으로 제공하는데

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
```

위에 코드는 두가지 문제가 있다. Parameter가 계속 copy된다는 것


```cpp
#include <iostream>

#define __LOGGING__ std::cout << __PRETTY_FUNCTION__ << '\n';

template<typename... Unpaked>
struct Pack {
	Pack() { __LOGGING__ } 
};

template<typename Front, typename... Rest>
struct Pack<Front, Rest...> : public Pack<Rest...> {
	using Type = Front;
	using Base = Pack<Rest...>;

	Pack(Front front, Rest... rest): Base(rest...), value(front) { __LOGGING__ }

	Type value;
};

struct Noisy
{
	Noisy() { std::cout << "constructed\n"; }
	Noisy(const Noisy&) { std::cout << "copy-constructed\n"; }
	Noisy &operator=(const Noisy&) { std::cout << "copy-assigned\n"; }
	Noisy(Noisy&&) { std::cout << "move-constructed\n"; }
	Noisy &&operator=(Noisy&&) { std::cout << "move-assigned\n"; }
	~Noisy() { std::cout << "destructed\n"; }
};

int main() {
	Pack<> p0();
	Pack<int> p1(2);
	Pack<int, char> p2(1, 'c');

	Noisy n0, n1, n2;
	Pack<Noisy, Noisy, Noisy> p3(n0, n1, n2);

	return 0;
}

```

[1]: http://en.cppreference.com/w/cpp/language/parameter_pack
