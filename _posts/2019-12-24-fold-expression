---
layout: post
title: Fold Expression from C++17 
tags: [c++]
---

C++17에 도입된 Fold Expression에 대하여 알아보자.
![c++17](https://www.modernescpp.com/images/blog/Functional/FunctionalInCpp17andCpp20/timeline.FunktionalInCpp17Cpp20Eng.png)
>  [cppreference]([https://en.cppreference.com/w/cpp/language/fold](https://en.cppreference.com/w/cpp/language/fold))  
> Reduces (folds) a parameter pack over a binary operator.

Fold expression은 c++11에 도입된 parameter pack을 줄여서 표현 할 수 있는 표현식이다. 이를 알아보기 전에 fold라는 개념에 대해 정리하고 넘어가자.

# higher-order function
함수형 프로그래밍에서 함수는 고차 함수(higher-order)과 일차 함수(first order)로 나뉜다. 고차 함수는 함수를 parameter로 받거나, return형으로 반환 형태를 칭한다. (일차 함수는고차 함수가 아닌 모든 함수를 이야기한다.)  

고차 함수가 왜 필요할까?  

그 이유 중 하나는 고차 함수의 인자로 들어온 함수를 통해 다른 인자(데이터)를 반복적(혹은 재귀적)으로 처리하기 위함이다.  [wiki]([https://en.wikipedia.org/wiki/Higher-order_function](https://en.wikipedia.org/wiki/Higher-order_function))에 나와 있는 고차 함수의 예를 통해 알아보면 이해가 수월 할 것이다.  
아래 예제는 int형 인자를 받아 3을 더하는 일차 함수 f를,  고차 함수 twice를 통해 두 번 반복하는 것을 나타낸다. 결과 값은 (7 + 3) + 3 = 13이다.
```cpp
// Generic lambdas provided by C++14.
#include <iostream>

auto twice = [](auto f, int v)
{
    return f(f(v));
};

auto f = [](int i)
{
    return i + 3;
};

int main()
{
    std::cout << twice(f, 7) << std::endl;
}
```

# [fold]([https://en.wikipedia.org/wiki/Fold_(higher-order_function)](https://en.wikipedia.org/wiki/Fold_(higher-order_function)))
Fold는 고차 함수 계열 중 하나로, 함수의 인자에 대하여 **특정 연산자에 대해** 재귀적으로 처리한 결과 값을 반환한다.  

아래 그림은 데이터 구조 1, 2, 3, 4, 5, [] 그리고 초기 값 z에 대하여 right-fold로 방식으로 함수 f를 적용한 결과 값을 구하는 것이다. (여기서 []는 empty, 혹은 초기 값을 나타 낼 수 있다.)

![right-fold](https://upload.wikimedia.org/wikipedia/commons/3/3e/Right-fold-transformation.png)

위를 해석 하면, 함수 f는 int와 자신을 인자로 갖는 함수이며, 데이터 구조의 끝(empty)을 나타내는 []에 대하여 초기 값 z를 적용한다. c++ 코드로 아래와 같다.
```cpp
auto result = f(1, f(2, f(3), f(4, f(5, f())); // f() == z
```

원론적인 용어 부분은 간략히 정리가 되었고, fold expression이 도입된 이유에 대해 생각해 보자.

# parameter pack
c++17에 왜 fold의 개념이 도입했는지는 c++11에 도입된 parameter pack을 보면 이해할 수 있다.  
Parameter pack은 variadic template을 통해 가변 인자를 받을 수 있도록 도입 된 것인데, 이를 이용하여 sum을 계산하면 아래와 같다.

```cpp
#include <iostream>

auto sum()
{
    return 0;
}

template <typename Front, typename... Rest>
auto sum(Front front, Rest&&... rest)
{
    return front + sum(std::forward<Rest>(rest)...);
}

int main()
{
    std::cout << sum(1, 2, 3, 4, 5) << std::endl;
    return 0;
}
```
위와 같이 parameter가 없을 때 default value를 리턴해주는 함수를 추가로 만들어주어야, 재귀적으로 종료 조건이 성립한다.  

즉, 하나의 기능을 구현하기 위하여 두가지 형식으로 작성해야한다.

# [fold expression]([https://en.cppreference.com/w/cpp/language/fold](https://en.cppreference.com/w/cpp/language/fold))
위의 parameter pack을 통해 sum을 구한 예제를 fold expression을 사용하면 아래와 같이 간단해진다.

```cpp
#include <iostream>

template <typename... Args>
auto sum(Args&&... args)
{
    // same with below: return (args + ...);
    return (args + ... + 0);
}

int main()
{
    std::cout << sum(1, 2, 3, 4, 5) << std::endl;
    return 0;
}https://en.wikipedia.org/wiki/Fold_(higher-order_function)

/// 1 + (2 + (3 + (4 + (5 + 0)))) = 15
```
정확히는 위는 binary right fold 방식을 적용한 것으로, 이 외에 아래와 같은 문법이 존재한다. 이들은 fold의 방식이 다를뿐 원론적인 부분은 같다.
### Syntax
1.  unary right fold
2.  unary left fold
3. **binary right fold**
4.  binary left fold

# Summary
c++17에서는 함수형 프로그래밍에 존재하는 fold expression의 개념을 가져와 기존의 parameter pack을 좀 더 편리하게 표현 할 수 있게 하였다.
