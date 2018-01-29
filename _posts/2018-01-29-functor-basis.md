### [Function Object][1]
> A function object is any object for which the function call operator is defined.  

Function Object(이하 functor)는 객체에 함수 호출 연산자를 더한 것을 의미한다. 다른 의미로 풀어 
쓰면 함수 호출 연산자 '( )'가 정의 된 모든 객체를 의미한다.  

- object + () 

이 것은 아래의 네가지로 표현 될 수 있다.

1. normal function
2. function pointer
3. class object which the () operator is overloaded
4. lambda expression

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
#### function pointer
함수 포인터는 함수의 시작 번지를 가르키는 포인터이다. 기본적으로 함수는 인자와 리턴 값을 가지고 
시그니처를 결정하는데 함수 포인터는 아래와 같은 형식을 갖는다.

```cpp
    [return type] (*funtion pointer name)[argument types...]
```

### [predicate with stl][2]
> STL refines functor concepts as follows:  
> A generator is a functor that can be called with no argument.  
> A unary function is a functor that can be called with one argument.  
> A binary function is a functor that can be called with two arguments.  

[1]:http://en.cppreference.com/w/cpp/utility/functional
[2]:http://www.bogotobogo.com/cplusplus/functors.php
