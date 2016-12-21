---
layout: post
title:  noexcept keyword
tags: [c++] 
---

# C++11 noexcept keyword

## [noexcept operator (since C++11)][1]
> The noexcept operator performs a compile-time check that returns true  
> if an expression is declared to not throw any exceptions.    
> It can be used within a function template's noexcept specifier  
> to declare that the function will throw exceptions for some types but not others.  
  

C++11 부터 추가된 keyword로 interface 설계시 중요한 역할을 한다.    
- Client에게 해당 함수가 예외처리가 필요한지 아닌지에 대한 정보를 전달      
- C++98엔 throw keyworld로 조절 했었음

## [non-throwing function (since c++)][2]  
기본적으로 예외를 던지지 않는 함수들  
- destructor
- deallocation function
- ...

소멸자의 경우 default로 noexcept가 선언되어 있으며 이를 무시하고  
소멸자에서 예외를 던져야 한다면 명시적으로 noexcept(false)를 선언해주어야 한다.
  
[1]: http://en.cppreference.com/w/cpp/language/noexcept
[2]: http://en.cppreference.com/w/cpp/language/noexcept_spec
