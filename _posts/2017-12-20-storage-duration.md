---
layout: post
title: 객체의 유효 범위
tags: [c++]
---

객체가 생성이되고 실제 메모리를 할당 받으면 인스턴스라한다. 각 객체, 인스턴스가 유효한 범위에 대해 알아보자.

### [Storage duration][1]
프로그램 내 저장된 모든 객체들은 아래중 하나에 속하게 된다.

- automatic storage duration: 
지역변수로 선언된 객체는 중괄호로 감싸진 하나의 코드-블락 내에서 유효하다. 
stack discipline에 의해 함수가 호출 되고 스택 프레임이 생성되면 지역변수는 메모리를 할당 받으며 함수가 종료 되면서 메모리가 해제된다.

- static storage duration: 
static이나 extern으로 선언된 객체는 프로그램이 시작될 때 메모리를 할당받고 프로그램이 종료될 때 메모리가 해제 된다. 
모든 객체는 글로벌 네임스페이스를 포함한 해당 네임스페이스의 스코프에서 선언 된다. 또한 객체는 하나의 인스턴스만 갖게 된다.

- thread storage duration(since c++11): 
tread_local로 선언된 객체들은 해당 스레드가 생성되면서 메모리를 할당 받고 스레드 종료와 함께 메모리가 해제 된다. 
각각의 스레드는 객체에대해 고유한 인스턴스를 갖는다.

- dynamic storage duration: 
동적 할당된 변수들이 유효한 범위이다.

[1]:http://en.cppreference.com/w/cpp/language/storage_duration
