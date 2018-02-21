---
layout: post
title: qxx (Query++, type-safe query builder)
tags: [c++] 
---

# QXX (Query++, type-safe query builder)

Header | Technique brief
------------- | ------------
column.hxx | Capture member pointer of struct
type.hxx, condition.hxx | Distinguish type in differenct scope and make type-safety
table-impl.hxx, database-impl.hxx | Hold differents types
tuple-helper.hxx. table.hxx, database.hxx | Iterate tuple with closure
expression.hxx, condition.hxx | Define expression and conditions


## Capture member pointer of struct

- pointer to member (object, function)
- :: (scope resolution operator)
- \* (address-of operator)
- & (indirect operator)
	
```cpp
struct Person {
	int id; // int Person::*
	int age; // int Persson::*
	std::string name; // std::string Person::*
};
```

[1]:https://review.tizen.org/gerrit/#/c/168105/2/include/klay/db/query-builder/column.hxx

## Distinguish type in differenct scope and make type-safety
- static_assert
- reinterpret_cast
- type_traits
- data structure alignment

```cpp
// g++ main.cpp -std=c++11
#include <iostream>
#include <string>
#include <type_traits>

/* Person
--------------------
|int | int | string |
--------------------
*/

struct Person {
	int id;
	int age;
	std::string name;
};

struct Animal {
	int id;
	int age;
	std::string name;
};

template<typename L, typename R>
void compare_between_same_scope(L left, R right)
{
	if (left == reinterpret_cast<L>(right))
		std::cout << "Two type is same." << '\n';
	else
		std::cout << "Two type is not same." << '\n';
}

template<typename L, typename R>
void compare_between_different_scope(L left, R right)
{
	if (std::is_same<L, R>::value)
		std::cout << "Two type is same." << '\n';
	else
		std::cout << "Two type is not same." << '\n';
}

int main()
{
	int Person::*personInt1 = &Person::id;
	int Person::*personInt2 = &Person::age;

	int Animal::*animalInt1 = &Animal::id;
	int Animal::*animalInt2 = &Animal::age;

	compare_between_same_scope(personInt1, personInt2);
	compare_between_same_scope(animalInt1, animalInt2);

	compare_between_different_scope(personInt1, animalInt1);
	compare_between_different_scope(personInt1, animalInt2);
	compare_between_different_scope(personInt2, animalInt1);
	compare_between_different_scope(personInt2, animalInt2);

	// false positive
	compare_between_same_scope(personInt1, animalInt1);
	compare_between_different_scope(personInt1, personInt2);

	return 0;
}
```

## Hold differents types
- variadic template
- recursive template

```cpp
// g++ main.cpp -std=c++11
#include <iostream>

template<typename... Unpaked>
struct Pack {
	Pack() {}
};

template<typename Front, typename... Rest>
struct Pack<Front, Rest...> : public Pack<Rest...> {
	using Type = Front;
	using Base = Pack<Rest...>;

	Pack(Front front, Rest... rest): Base(rest...), value(front) {}

	Type value;
};

// helper method
template<typename... Args>
Pack<Args...> make_pack(Args&&... args) {
	return Pack<Args...>(std::forward<Args>(args)...);
}

int main() {
	struct Data {};
	
	Pack<> p0();
	Pack<int> p1(2);
	Pack<int, char> p2(1, 'c');
	Pack<int, char, Data> p3(1, 'c', Data());
	
	auto easy = make_pack(1, 'c', Data());

	return 0;
}
```
## Iterate tuple with closure
- tuple iterator
- generic lambda (since c++14)
- functor
- () (function call operator)

```cpp
#include <iostream>
#include <vector>

template<typename C, typename F>
void for_each(C container, F functor)
{
	for (const auto& c : container)
		functor(c);
}

// 1. normal function
void normalFunc(int val) { std::cout << val << '\n'; }

int main() {
	std::vector<int> ints {1, 2, 3, 4};

	// 2. function pointer
	void (*funcPtr)(int) = normalFunc;

	// 3. class object
	struct Functor {
		void operator()(int val) { std::cout << val << '\n'; }
	};

	for_each(ints, normalFunc);
	for_each(ints, funcPtr);
	for_each(ints, Functor()); 

	// 4. lambda expression
	for_each(ints, [](int val) { std::cout << val << '\n'; });
};
```
## Define expression and conditions
- expression (name == "kwon", age > 20)
- conditions (AND, OR)
- Inheriting constructor
- SFINAE (Substitution Failure Is Not An Error)

```cpp
struct B1 {
    B1(int);
};
struct D1 : B1 {
    using B1::B1;
// The set of candidate inherited constructors is 
// 1. B1(const B1&)
// 2. B1(B1&&)
// 3. B1(int)
 
// D1 has the following constructors:
// 1. D1() = delete
// 2. D1(const D1&) 
// 3. D1(D1&&)
// 4. D1(int) <- inherited
};
```
