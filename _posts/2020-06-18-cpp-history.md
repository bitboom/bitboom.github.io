---
layout: post
title: How C++ flow?
tags: [c++]
---

[C++20](https://en.cppreference.com/w/cpp/20)에 대한 윤곽이 잡히고 있다.
C++은 어떻게 변화해가고 있는지 살펴보자.

# [C++ 표준의 역사](http://www.bloter.net/archives/291766)
우리가 흔히 이야기하는 C++은 C++03이다.
C++은 1983년도에 처음 모습을 드러내었고
1998년에 첫 표준화인 C++98을 거쳐,
2003년도에 버그들을 수정하여 C++03가 나왔다.

이후 다른 언어라고까지 표현되는 Modern C++은
2011년도에 표준화된 C++11부터를 나타내며,
마이너 업데이트인 C++14를 포함한다.
Modern C++ 이후부터는 3년마다 새로운 표준을 내놓는
'기차 모델'을 따른다.

아래의 그림의 초록색은 TS(Technical Specification)을 나타낸다.
TS가 충분히 논의되고 합의에 이르면 다음 버전에 포함되는데,
이를 트렁크에 포함되었다고 표현한다.
아래의 파랜색은 트렁크를 나타낸다.

![cpp pipeline](https://herbsutter.files.wordpress.com/2016/03/wg21-timeline.png?w=1100)

# [C++11](https://en.cppreference.com/w/cpp/11)
Modern C++이라 표현되는 C++11은 thread, type_traits를
포함하여 무려 13개의 헤더가 추가되었다.
대표적인 새로운 기능은 move, lambda expression, rvalue reference, smart pointer이며
이 외에도 중요한 문법적 요소들이 많이 추가 되었다.

- auto and decltype
- defaulted and deleted functions
- final and override
- trailing return type
- rvalue references
- move constructors and move assignment operators
- variadic templates
- lambda expressions
- noexcept specifier and noexcept operator
- multithreaded memory model
- thread-local storage
- range-for (based on a Boost library)
- static_assert (based on a Boost library)
- ...

# [C++17](https://en.cppreference.com/w/cpp/17)
C++17은 새로운 에러처리 방식인 optional부터 filesystem등
8개의 헤더가 추가 되었다.
새로운 헤더뿐 아니라 문법적 요소를 포함한 새로운 기능들이 추가되었다.
아래는 그 중 몇가지를 선정하여 나열한 것이다.

- fold-expressions
- class template argument deduction
- auto non-type template parameters
- compile-time if constexpr
- inline variables
- structured bindings
- initializers for if and switch
- ...

# References
- [‘기차모델’로 갈아탄 ‘C++17’이 가져온 변화는?](http://www.bloter.net/archives/291766)
- [Trip report: Winter ISO C++ standards meeting](https://herbsutter.com/2016/03/11/trip-report-winter-iso-c-standards-meeting/)
