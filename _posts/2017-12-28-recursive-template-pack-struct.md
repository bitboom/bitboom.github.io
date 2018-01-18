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

## Tutorial

#### LEVEL 1 : Type safe variadic template
- variadic template은 직접 순회를 할 수 없음
- recursive template를 이용해야함 

```cpp
void process(void) // base condition
{
	std::cout << "Base condition: non-parameter" << '\n';
}

template<typename T1, typename... Tn>
void process(T1&& arg1, Tn&&... args)
{
	handle(std::forward<T1>(arg1));
	process(std::forward<Tn>(args)...);
}
```
#### LEVEL 2 : Prohibit empty parameter
- LEVEL 1은 개수가 0인 parameter를 허용함
- 모든 process의 호출은 개수의 상관없이 base condition을 호출함

```cpp

void handle(int value) { std::cout << "interger: " << value << '\n'; }
void handle(const std::string& value) { std::cout << "string: " << value << '\n'; }
void handle(double value) { std::cout << "double: " << value << '\n'; }

template<typename T>
void process(T&& arg)
{
	handle(arg);
}

template<typename T1, typename... Tn>
void process(T1 arg1, Tn.. args)
{
	handle(arg1);
	process(args...);
}

// process(1, 2, 4.3, "test");
```

#### LEVEL 3 : Perfect forwaridng
- LEVEL 2은 parameter의 복제가 일어남
- literal형은 const 참조만 받을 수 있기에 참조형으로 인자를 선택할 수 없음

```cpp
void handle(int value) { std::cout << "interger: " << value << '\n'; }
void handle(const std::string& value) { std::cout << "string: " << value << '\n'; }
void handle(double value) { std::cout << "double: " << value << '\n'; }

template<typename T>
void process(T&& arg)
{
	handle(std::forward<T>(arg));
}

template<typename T1, typename... Tn>
void process(T1&& arg1, Tn&&... args)
{
	handle(std::forward<T1>(arg1));
	process(std::forward<Tn>(args)...);
}

// process(1, 2, 4.3, "test");
```

#### LEVEL 4 : As class
```cpp
template<typename... Base>
class Variadic {
	explicit Variadic() {} 
};

template<typename Front, typename... Rest>
class Variadic<Front, Rest...> : private Variadic<Rest...> {
public:
	using Type = Front;

	explicit Variadic(Front front, Rest... rest) : Base(rest...), value(front) {} 

	Type value;

private:
	using Base = Variadic<Rest...>;
}; 
```

#### LEVEL 5 : Apply lambda
```cpp
template<typename... Unpaked>
struct Pack {
	int size() const noexcept { return 0; }

	template<typename Closure>
	void apply(Closure&& closure) {}
};

template<typename Front, typename... Rest>
struct Pack<Front, Rest...> : public Pack<Rest...> {
	using Type = Front;
	using Base = Pack<Rest...>;

	Pack(Front front, Rest... rest) : Base(rest...), value(front) {}

	int size() const noexcept { return Base::size() + 1; }

	template<typename Closure>
	void apply(Closure&& closure) {
		Base::apply(std::forward<Closure>(closure));
		closure(this->value);
	}

	Type value;
};

template<typename... Args>
Pack<Args...> make_pack(Args&&... args) {
	return Pack<Args...>(std::forward<Args>(args)...);
}

/* test

	struct Data {
		int id;
	};

	Data n0{0}, n1{1}, n2{2};
	auto pack = make_pack(n0, n1, n2);
	std::cout << pack.size() << std::endl;

	pack.apply([](Data d){
			std::cout << "Closure: " << d.id << std::endl;
		});

*/
```

#### LEVEL 6 : Apply generic lambda in c++11
- LEVEL5는 pack의 인자로 같은 타입의 lambda만 가능
- c++11은 generic lambda를 사용할 수 없기에
- c++11에서 사용 가능하려면 Closure를 흉내 내어서 구현

```cpp
#include <iostream>

#include <vector>
#include <string>
#include <tuple>

struct Data {
	int id;
};

struct Data2 {
	int id;
};

template<typename... Unpaked>
struct Pack {
	int size() const noexcept { return 0; }

	template<typename Closure>
	void apply(Closure&& closure) {}
};

template<typename Front, typename... Rest>
struct Pack<Front, Rest...> : public Pack<Rest...> {
	using Type = Front;
	using Base = Pack<Rest...>;

	Pack(Front front, Rest... rest) : Base(rest...), value(front) {}

	int size() const noexcept { return Base::size() + 1; }

	template<typename Closure>
	void apply(Closure&& closure) {
		Base::apply(std::forward<Closure>(closure));
		closure(this->value);
	}

	Type value;
};

template<typename... Args>
Pack<Args...> make_pack(Args&&... args) {
	return Pack<Args...>(std::forward<Args>(args)...);
}

struct Closure {
	template<typename Type>
	void operator()(Type value) {
		std::cout << "Closure: " << value.id << std::endl;
	}
};

int main() {
	Data n0{0}, n1{1};
	Data2 n2{3};
	auto pack = make_pack(n0, n1, n2);
	std::cout << pack.size() << std::endl;

	pack.apply(Closure());

	return 0;
}
```
#### LEVEL 7 : Hiding information of Base class
```cpp
#include <vector>
#include <string>
#include <tuple>

struct Data {
	int id;
};

struct Data2 {
	int id;
};

template<typename... Unpaked>
struct Pack {
	int size() const noexcept { return 0; }

	template<typename Closure>
	void apply(Closure&& closure) {}
};

template<typename Front, typename... Rest>
struct Pack<Front, Rest...> : public Pack<Rest...> {
	using Type = Front;

	int size() const noexcept { return Base::size() + 1; }

	Pack(Front front, Rest... rest) : Base(rest...), value(front) {}

	template<typename Closure>
	void apply(Closure&& closure) {
		Base::apply(std::forward<Closure>(closure));
		closure(this->value);
	}

	Type value;

private:
	using Base = Pack<Rest...>;
};

template<typename... Args>
Pack<Args...> make_pack(Args&&... args) {
	return Pack<Args...>(std::forward<Args>(args)...);
}

struct Closure {
	template<typename Type>
	void operator()(Type value) {
		std::cout << "Closure: " << value.id << std::endl;
	}
};

int main() {
	Data n0{0}, n1{1};
	Data2 n2{3};
	auto pack = make_pack(n0, n1, n2);
	std::cout << pack.size() << std::endl;

	pack.apply(Closure());

//	Pack<Data, Data, Data2> dpack(n0, n1, n2);
//	std::cout << dpack.size() << std::endl;

//	Pack<Data, Data, Data2>::Base basePack(Data(), Data());

	return 0;
}
```
[1]: http://en.cppreference.com/w/cpp/language/parameter_pack
