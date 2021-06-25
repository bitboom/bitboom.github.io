
---
layout: post
title: Serialization and Marshalling
tags: [common]
---

##  직렬화(Serialization)는 형변환이다.
데이터를 파일로 저장하거나, 네트워크를 통해 전송 하려면 약속된 형태로 변환해야 한다. 네트워크를 통해 데이터를 보내려면, 송신부는 데이터를 바이너리 스트림로 변환하여 네트워크 장치를 통해 전송한다. 수신부는 바이너리 스트림을 수신하면 역직렬화(Deserialization)을 통해 데이터를 복구할 수 있다.

```
  << Sender >>                     << Receiver >>
| Data -> Binary | ⇒ Network => | Binary -> Data |
```

## 메모리 입장에서 모든 데이터는 0과 1일 뿐
코드를 작성할 때 프로그래밍 언어의 타입 시스템을 이용하여 **데이터에 타입을 부여하지만, 메모리 입장에선 바이너리** 일 뿐이다. 특정 메모리에 올라와 있는 값들을 컴파일러가 언어의 타입에 맞게 해석 할 뿐이다. 그렇다면 모든 데이터는 바이너리 배열로 나타낼 수 있다는 의미다. 

다시 말해, 직렬화는 바이너리 배열로 형변환 하는 것이다. 
```
  << Sender >>                                          << Receiver >>
| struct -> std::vector<unsigned char> | ⇒ Network => | std::vector<unsigned char> -> struct |
```

## 코드로 표현하면 형변환(Casting)의 연속 
```cpp
// serialization
Foo foo;
auto ptr = reinterpret_cast<unsigned char*>(&foo);
std::vector<unsigned char> binary(ptr, ptr + sizeof(foo));

// deserialization
Foo restore;
std::memcpy(reinterpret_cast<void*>(&restore), reinterpret_cast<void*>(binary.data()), sizeof(restore));
```

## 모든 구조체를 단순히 형변환 할 순 없다.
구조체는 멤버 변수들만 담고 있는 것이 아니다. vtable을 포함하여 다른 meta 정보가 들어 갈 수 있다. 이는 일련의 메모리 영역에 함께 저장되는 것이 아니기 때문에 단순히 형변환 할 수 없다 (vtable을 가르키는 주소만 가지고 있기 때문). 단순히 형변환 할 수 있는 구조체는 [PODType](https://en.cppreference.com/w/cpp/types/is_pod) 정도이다.

## 그렇다면 모든 멤버 변수를 순회하면 된다.
모든 구조체를 커버하는 직렬화 라이브러리를 구현하려면, 모든 멤버 변수를 순회 하면서 바이너리 스트림에 이어 붙이면 된다. Rust는 컴파일러의 기능을 이용할 수 있는 매크로가 지원 되기 때문에, 간단하게 순회 하면서 바이너리 스트림을 만들 수 있다.

C++은 각 멤버를 직접 순회해야한다. [Archive](https://github.com/bitboom/rmi/blob/devel/src/stream/archive.hxx)는 재사용 가능하게 직렬화를 할 수 있도록 작성해 놓은 라이브러리다.

## 여담: 마샬링은 직렬화의 또 다른 이름
마샬링(marshalling)은 직렬화의 또 다른 이름이다. 객체를 바이너리로 변환하는 과정을 마샬링이라고 하기도 한다.

다른 관점에선 마샬링을 더 큰 개념으로 보기도 한다. 역/직렬화가 같은 언어 혹은 다른 언어 간에 프로토콜로 데이터를 바이너리로 표현하는 것이라면, 언/마샬링은 언어를 떠나 다른 플랫폼 간의 프로토콜을 나타내기도 한다.
