---
layout: post
title: 데이터 타입과 레이아웃에 대해
tags: [c++]
---

네트워크를 통해 데이터를 전송하거나 다른 언어간 데이터를 변환 할 때는 데이터를 직렬화(Serialize)한다.
데이터 타입을 바이너리 스트림으로 변환 하는 것이다.
`int, double`과 같은 스칼라 타입의 경우 형변환 만으로도 직렬화가 가능하다.
이는 메모리에 저장될 때 연속된 공간에 저장 되기 때문이다.

레이아웃(Layout)은 데이터가 메모리에 어떠한 형태로 저장 되는지를 표현한다.
C언어의 구조체의 경우도 컴파일러가 정한 규칙에 따라 연속된 공간에 저장되기 때문에 간단한 형변환으로 직렬화 가능하다.

C++특징을 사용한 구조체 들은 어떨까? C++은 가상함수, 상속과 같은 개념이 있고 이는 레이아웃을 복잡하게 만든다.
레이아웃이 복잡하고 고정될 수 없기 때문에 C와 같은 다른 언어로 작성된 프로그램과 호환 될 수 없고,
인접하지 않을 수도 있기 때문에 memcopy와 같은 빠른 하위 수준 함수를 사용할 수 없다.
따라서 Binary I/O 혹은 네트워크 통신에 직접적으로 사용 할 수 없다.

C++14 이러한 타입을 추론하기 위해 간단한 클래스와 구조체인 trivial ,
표준 레이아웃 및 POD 또는 Plain Old Data의 세 가지 범주를 도입했습니다.

`is_trivial<T>, is_standard_layout<T> 그리고 is_pod<T>`를 통해 타입을 추론 할 수 있다.

*본 글은 해당 [포스트](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170)를 번역, 해석하였다.*

## Standard Layout Type
표준 레이아웃 타입은 C와 호환 가능한 구조체이다.
가상 함수와 같은 C에 없는 특징들을 사용하지 않은 구조체 혹은 클래스이기 떄문이다.

따라서 표준 레이아웃 타입은
- memcpy()를 사용할 수 있고,
- C++과 C언어에서 호환 가능하다.

표준 레이아웃을 만족하려면 아래의 특성을 만족해야한다.
- 가상함수나 가상 클래스가 아니여야 한다
- 모든 데이터 멤버는 같은 Access Control을 가져야 한다. (Public, private or protected)
- 모든 데이터 멤버는 표준 레이아웃 타입이여야한다.
- 부모 클래스는 표준 레이아웃 타입이여야한다.
- 부모 클래스에 데이터 멤버가 없어야 한다.

마지막 항목의 예제는 아래와 같다.
```cpp
struct Base
{
   int i;
   int j;
};

— 

// std::is_standard_layout<Derived> == false!
struct Derived : public Base
{
   int x;
   int y;
};

struct Base
{
   void Foo() {}
};

// std::is_standard_layout<Derived> == true
struct Derived : public Base
{
   int x;
   int y;
};
```

##  Trivial Type
Trivial을 번역하면 하찮다는 뜻이 나오는데, 여기서 사용 되는 것은 평함하다, 사소하다는 뜻에 가깝다.
생성자, 소멸자, 복사+이동 생성자가 컴파일러가 제공하는 형태이면 Trivial Type이다.

Trivial Type은
- 암묵적으로 생략하여 컴파일러가 제공하는 함수를 사용하거나,
- 명시적으로 `default` 키워드를 사용하는 경우를 포함한다.

컴파일러가 제공하는 함수를 사용한다는 것은 컴파일러가 메모리에 어떠한 형태로 저장할 수 있는지 결정 할 수 있다는 의미이다.
표준 레이아웃 타입과 달리 데이터 멤버들이 다른 Access Control을 가질 수 있다.
컴파일러가 데이터 멤버의 Access Control의 정보를 알기 때문에 원하는 형태로 메모리에 정렬할 수 있기 때문이다.

따라서 Trivial Type은
- memcpy()를 사용할 수 있고,
- C++과 C언어에서 호환 **불가능**하다.

Trivial Type은 아래의 조건을 만족해야한다.
- 가상 함수가 없고 가상 클래스를 상속 받지 않아야 한다.
- 부모 클래스에서 non-trivial contractor/operator/destructor가 없어야한다.
- 데이터 멤버는 trivial type 이여야 한다.

아래의 예제는 Trivial Type이다.
```cpp
struct Trivial
{
   int i;
private:
   int j;
};

struct Trivial2
{
   int i;
   Trivial2(int a, int b) : i(a), j(b) {}
   Trivial2() = default;
private:
   int j;   // Different access control
};
```

## POD types
구조체가 trivial type과 표준 레이아웃을 모두 갖추면 POD(Plain Old Data)라 부른다.
POD타입은 연속된 메모리에 저장되고 멤버들이 선언된 순서대로 저장된다.

**POD 타입은 바이트 단위로 복사되어 Binary I/O로 수행 될 수 있다.**
스칼라 타입들은 POD타입이다.

아래의 예제를 통해 각 데이터 타입들을 체크해보자.

```cpp
#include <type_traits>
#include <iostream>

using namespace std;

struct B
{
protected:
   virtual void Foo() {}
};

// Neither trivial nor standard-layout
struct A : B
{
   int a;
   int b;
   void Foo() override {} // Virtual function
};

// Trivial but not standard-layout
struct C
{
   int a;
private:
   int b;   // Different access control
};

// Standard-layout but not trivial
struct D
{
   int a;
   int b;
   D() {} //User-defined constructor
};

struct POD
{
   int a;
   int b;
};

int main()
{
   cout << boolalpha;
   cout << "A is trivial is " << is_trivial<A>() << endl; // false
   cout << "A is standard-layout is " << is_standard_layout<A>() << endl;  // false

   cout << "C is trivial is " << is_trivial<C>() << endl; // true
   cout << "C is standard-layout is " << is_standard_layout<C>() << endl;  // false

   cout << "D is trivial is " << is_trivial<D>() << endl;  // false
   cout << "D is standard-layout is " << is_standard_layout<D>() << endl; // true

   cout << "POD is trivial is " << is_trivial<POD>() << endl; // true
   cout << "POD is standard-layout is " << is_standard_layout<POD>() << endl; // true

   return 0;
}
```
