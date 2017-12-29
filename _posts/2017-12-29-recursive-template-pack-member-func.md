---
layout: post
title: Recurisve Template에 member function 추가하기 (2)
tags: [c++] 
---

Recursive template를 활용하여 pack struct 구현 이후 해당 struct에 member function을 구현해보자.
우선적으로 pack의 size를 구현하는 것은 재귀 호출을 통하여 구현이 가능하다.  

하지만 generic하게 Pack의 member variable에 적용할 수 있도록 함수 포인터를 넘기는 것은 C++11에선 매우 제한적으로 구현이 가능하다. 여기서 제한적이라 함은 pack의 인자로 들어온 타입과 apply()의 함수 포인터에서의 인자 타입과 같아야 한다는 것이다.

```cpp
#include <iostream>
#include <utility>

template<typename... Unpaked>
struct Pack {
	int size() const noexcept { return 0; }

	template<typename Func>
	void apply(Func) {}
};

template<typename Front, typename... Rest>
struct Pack<Front, Rest...> : public Pack<Rest...> {
	using Type = Front;
	using Base = Pack<Rest...>;

	Pack(Front front, Rest... rest) : Base(rest...), value(front) {}

	int size() const noexcept { return Base::size() + 1; }

	// Very restrictive : since we can us generic lambda in c++11 
	template<typename Func>
	void apply(Func func) {
		func(this->value);
		Base::apply(func);
	}

	Type value;
};

template<typename... Args>
Pack<Args...> make_pack(Args&&... args) {
	return Pack<Args...>(std::forward<Args>(args)...);
}

struct Data {
	int id;
};

int main() {
	Data n0{0}, n1{1}, n2{2};
	auto pack = make_pack(n0, n1, n2);
	std::cout << pack.size() << '\n';

	auto empty = make_pack();
	std::cout << empty.size() << '\n';

	pack.apply([](Data data) { std::cout << ++data.id << std::endl; });

	return 0;
}
```
[1]: http://en.cppreference.com/w/cpp/language/parameter_pack
