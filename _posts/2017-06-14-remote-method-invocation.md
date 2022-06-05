---
layout: post
title:  Remote Method Invocation
tags: [framework]
---

### RMI, Remote Method Invocation

Client에서 Server의 함수를 호출하는 기법.
Server의 함수에 parameter를 넘겨주고 return 값을 받음.

RMI 통신 방법으로 객체 직렬화 (Serialization) 이용함.
// 주소공간에서 다른 주소공간으로 객체의 전달을 투명하게 하기 위한 기술

1. Server에 객체(method를 들고 있음)를 식별자 이름과 같이 registery에 넣어두고 (binding)  
2. Client에서 접근한 후 Look-up을 과정을 통해 remote object를 얻고  
3. 이를 remote interface를 통하여 casting후 method invocation 즉, 객체의 메서드를 호출한다.  

### 용어 정리
- *원격 객체(remote object)* : Client로부터 호출되어지는 메소드를 가지고 있는 객체.  
- *원격 인터페이스(remote interfaces)* : 원격 객체의 메소드를 선언하는 인터페이스.  
- *원격 메소드 호출(Remote Method Invocation)* : 원격 인터페이스 안에 정의된 리모트 객체의 메소드를 호출하는 것.  

### 분산 객체 모델 (객체가 server와 client에 분산 되어 있음.)  
- remote object는 *구현 클래스*와 상호작용 하는 것이 아니라 remote interface와 상호작용함.  
- 원격메소드 호출시 결과값들은 참조가 아닌 복사로서 전달.  
- 원격 객체는 원격구현이 복사가 되는 것이 아니라 참조값으로 값이 전달.  

### RMI 시스템 구조
- stub/skeleton layer  
- remote reference layer  
- transport layer  

> [Serialization][1]
> In computer science, in the context of data storage,  
> serialization is the process of translating data structures  
> or object state into a format that can be stored  
> (for example, in a file or memory buffer) or  
> transmitted (for example, across a network connection link)  
> and reconstructed later (possibly in a different computer environment).  

[1]:https://en.wikipedia.org/wiki/Serialization
