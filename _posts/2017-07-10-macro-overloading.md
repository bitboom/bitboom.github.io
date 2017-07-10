---
layout: post
title:  Macro overloading
tags: [c++]
---

### Macro overloading
다양한 이유에서 macro로 함수를 정의할 필요가 생긴다.
이때 macro를 overloading 할 수 있는 기법에대해서 알아보자.

#### level 1. 기본적인 macro overloading
Gcc에서 지원하는 Variadic_macro를 사용하여 구현하면 아래와 같다.

```cpp
#define FOO_1(_1) std::cout << "FOO_1" << std::endl; 
#define FOO_2(_1, _2) std::cout << "FOO_2" << std::endl; 

#define FOO_X(_1, _2, FUNC, ...) FUNC

#define FOO(...) FOO_X(__VA_ARGS__, FOO_2, FOO_1)(__VA_ARGS__)
```

#### level 2. empty argument 받기
Empty argument를 받기 위해선 ##__VA_ARGS__를 사용하여야 한다.

> Second, the ‘##’ token paste operator has a special meaning  
> when placed between a comma and a variable argument. If you write  
> #define eprintf(format, …) fprintf (stderr, format, ##__VA_ARGS__)  
> and the variable argument is left out when the eprintf macro is used,  
> then the comma before the ‘##’ will be deleted.

```cpp
#define FOO_0() std::cout << "FOO_0" << std::endl; 
#define FOO_1(_1) std::cout << "FOO_1" << std::endl; 
#define FOO_2(_1, _2) std::cout << "FOO_2" << std::endl; 

#define FOO_X(_0, _1, _2, FUNC, ...) FUNC

#define FOO(...) FOO_X(_, ##__VA_ARGS__, FOO_2, FOO_1, FOO_0)(__VA_ARGS__)
```

#### level 3. C++ 스타일로 바꾸기
C++에서는 __VA_ARGS__ 대신에 alias를 쓸 수 있다.

```cpp
#define FOO_0() std::cout << "FOO_0" << std::endl; 
#define FOO_1(_1) std::cout << "FOO_1" << std::endl; 
#define FOO_2(_1, _2) std::cout << "FOO_2" << std::endl; 

#define FOO_X(_0, _1, _2, FUNC, ...) FUNC

#define FOO(args...) FOO_X(_, ##args, FOO_2, FOO_1, FOO_0)(args)
```

#### level 4. Argument 갯수 세기
level 1~3을 응용하면 argument의 갯수를 셀 수 있다.
```cpp 
#define COUNT_ARGS(args...)  ARG_X(_, ##args, 5, 4, 3, 2, 1, 0)
#define ARG_X(_0, _1, _2, _3, _4, _5, N, ...)   N
```

#### level 5. Macro를 overloading 할 수 있게하는 macro 생성

```cpp 
#define COUNT_ARGS(args...)  ARG_X(_, ##args, 5, 4, 3, 2, 1, 0)
#define ARG_X(_0, _1, _2, _3, _4, _5, N, ...)   N

#define MACRO_OVERLOADING(macro, args...) MACRO_EXPAND(macro, COUNT_ARGS(args))(args)
#define MACRO_EXPAND(name, number_of_args) MACRO_GENERATE(name, number_of_args)
#define MACRO_GENERATE(name, number_of_args) name##number_of_args
/* usage
#define BAR(args...) MACRO_OVERLOADING(BAR, args)

#define BAR0() std::cout << "BAR0" << std::endl; 
#define BAR1(_1) std::cout << "BAR1" << std::endl; 
#define BAR2(_1, _2) std::cout << "BAR2" << std::endl; 
*/
``` 

https://stackoverflow.com/questions/3046889/optional-parameters-with-c-macros
https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html
