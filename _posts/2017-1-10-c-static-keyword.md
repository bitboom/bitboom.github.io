---
layout: post
title: c static keyword
tags: [c]
---

## [static keyword][1]
> - In the C programming language static is a reserved word controlling both  
> **lifetime** (as a static variable) and **visibility** (depending on linkage) - ("file scope", also known as "internal linkage").
> - In C, static is a storage class as are extern, auto and register.

> |Storage class|Lifetime|Visibility|
> |---|---|---|
> |extern|program execution|external (whole program)|
> |static|program execution|internal (translation unit only)|
> |auto, register|function execution|N/A|

- 위의 keyword를 사용할때 Lifetime, visibility(scope)를 고려.  
	- 즉, 해당 keyword로 선언한 변수가 얼마만큼 살아 있는지, 어디서 사용할 수 있는지.  
- static keyword의 Lifetime은 프로그램 실행시 항상 살아 있음. (.data 영역에 저장)

### In C
- **static local variable** : 함수 내 선언한 정적 변수는 해당 함수 내에서만 사용 가능.
- **static global variable** : 파일 내 전역으로 선언한 정적 변수는 해당 파일 내에서만 사용 가능.
- **static function** :  
	- only visible throughout that file ("file scope", also known as "internal linkage").

### In C++
- static member variable
- static method
- 개념상으로 크게 차이는 없고, 중요한 것은 class의 객체가 메모리를 할당받기 전에 딱 한번 생성이 됨.
- 따라서 객체 생성 후에 접근할 수 있는 일반 member 변수에 대해 접근 할 수 없음.




[1]:https://en.wikipedia.org/wiki/Static_(keyword)
