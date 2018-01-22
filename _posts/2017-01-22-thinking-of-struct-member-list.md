---
layout: post
title: Thinking of strut member list
tags: [c++] 
---

### 구조체 멤버 리스트에 대한 고찰
c++에서는 일련의 데이터를 묶어서 저장하기 위하여 사용자 정의 타입인 구조체를 이용한다. 
구조체를 구성하는 멤버 리스트들은 메모리에 순서대로 저장된다. ([Data structure alignment][1]에 
의하여 저장된다. 즉, padding이 들어가지만 순서는 유지 된다.)

아래와 같이 Person 구조체를 선언 하였다면 메모리에는 아래의 블럭 모양처럼 들어갈 것이다.

```cpp
struct Person {
	int id;
	int age;
	std::string name;

	static int total;
};

/* Person
--------------------
|int | int | string |
--------------------
*/
```

### 구조체 멤버에 대한 접근
#### 범위 결정 연산자
여기서 이야기하고자 하는 것은 기본적인 구조체 선언, 정의가 아닌 *구조체의 멤버들(변수, 함수)을 
담아 낼 수 있는 방법*에 이야기 하고자 한다. 이를 하는 이유는 ORM과 같이 객체간에 관계를 정립하기에 
중요한 방법으로 쓰이기 때문이다.

구조체의 멤버에 접근하는 연산자는 크게 세 종류가 있다.
- :: (scope resolution)
- . (member access)
- .* (->) (member access through pointer to member)

여기서 ::연산자는 해당 멤버가 어느 구조체에 속하는지 결정해주는 연산자이다. static 멤버 변수는 
선언과 함께 초기화를 해주어야 하는데 이 연산자를 사용하여 초기화 한다.

```cpp
int Person::total = 10; 
```
Person 구조체에 속하는 int형인 total 변수를 10으로 대입하라라는 뜻으로 풀이 할 수 있다.
초기화를 하지 않았을 때 "(.text+0x6): undefined reference to `Person::total`"와 같이 
에러가 나타는 것으로 보아 구조체의 정의는 text 섹션에 저장 된다는 것을 유추 할 수 있다.

#### 주소 참조 연산자
그렇다면 text섹션에 저장되어 있는 구조체의 정의를 주소 참조 연산자를 통하여 접근해 보자.

```cpp
int Person::*memberInt = &Person::id; 
```

Person 구조체에 int형 타입을 가르키는 memberInt 포인터를 선언 후 Person::id의 주소를 대입하였다.
여기서 핵심은 Person구조체 내에 int형 타입을 가리키는 포인터를 선언 할 수 있다는 것이다.  

즉, &Person::id는 int Person::** 의 타입을 가진다.

### 구조체 내에 같은 type 멤버에 대한 접근 및 비교
위 Person 구조체는 int형 멤버변수를 2개 가진다. 두 멤버의 주소 참조 연산자 결과는 (&Person::id, Person::age) 모두 (int Person::*)의 type을 가진다.

```cpp
int Person::*memberInt1 = &Person::id;
int Person::*memberInt2 = &Person::age;

if (std::is_same<decltype(memberInt1), decltype(memberInt2)>::value)
	std::cout << "Two member type is same." << '\n';
```

위와 같은 접근은 한 구조체 내에 같은 type에 대한 비교가 불가능하다.
같은 구조체 내에서 같은 타입을 가지고 있기 때문이다. 그럼 Person 구조체 내에서 id와 
age의 다른 것을 찾아 비교 하면 되는데, 구조체는 위에서 설명한 것에 따라 기본적으로
선언 순서대로 메모리에 저장 된다. 즉 특정 base부터 시작하여 Person이 저장 되겠지만
Persion::id가 Persion::age보다 먼저 저장이 될 것이다.

그렇기 때문에 reinterpret_cast를 이용하여 명시적으로 변환 후에 두 type을 비교 하면 된다.

```cpp
int Person::*memberInt1 = &Person::id;
int Person::*memberInt1_1 = &Person::id;
int Person::*memberInt2 = &Person::age;

if (std::is_same<decltype(memberInt1), decltype(memberInt2)>::value)
	std::cout << "Two type is same." << '\n';
else
	std::cout << "Two type is not same." << '\n';

if (memberInt1 == reinterpret_cast<decltype(memberInt1)>(memberInt2))
	std::cout << "Two type is same." << '\n';
else
	std::cout << "Two type is not same." << '\n';

if (memberInt1 == reinterpret_cast<decltype(memberInt1)>(memberInt1_1))
	std::cout << "Two type is same." << '\n';
else
	std::cout << "Two type is not same." << '\n';

/*
Two type is same.
Two type is not same.
Two type is same.
*/
```

### 다른 구조체 내에서 멤버 type에 대한 비교
reinterpret_cast의 비교를 다른 구조체 멤버간의 비교에서 사용하면 안된다. 
정해진 규칙에 따라 메모리에 저장되기 때문에 아래 Person 구조체와 Animal 구조체는 같은 
방식으로 저장된다.
```cpp
struct Person {
	int id;
	int age;
	std::string name;
};

struct Animal {
	int id;
	int age;
	std::string name;
};
```
즉, 특정 base로부터 같은 거리에 저장이 되기 때문에 reinterpret_cast가 아닌 <type_traits>의 
std::is_same을 통하여 두 타입을 비교하면 된다.

### 종합 예제
```cpp
// g++ main.cpp -std=c++11
#include <iostream>
#include <string>
#include <type_traits>

struct Person {
	int id;
	int age;
	std::string name;
};

struct Animal {
	int id;
	int age;
	std::string name;
};

template<typename L, typename R>
void compare_between_same_scope(L left, R right)
{
	if (left == reinterpret_cast<L>(right))
		std::cout << "Two type is same." << '\n';
	else
		std::cout << "Two type is not same." << '\n';
}

template<typename L, typename R>
void compare_between_different_scope(L left, R right)
{
	if (std::is_same<L, R>::value)
		std::cout << "Two type is same." << '\n';
	else
		std::cout << "Two type is not same." << '\n';
}

int main()
{
	int Person::*personInt1 = &Person::id;
	int Person::*personInt2 = &Person::age;

	int Animal::*animalInt1 = &Animal::id;
	int Animal::*animalInt2 = &Animal::age;

	compare_between_same_scope(personInt1, personInt2);
	compare_between_same_scope(animalInt1, animalInt2);

	compare_between_different_scope(personInt1, animalInt1);
	compare_between_different_scope(personInt1, animalInt2);
	compare_between_different_scope(personInt2, animalInt1);
	compare_between_different_scope(personInt2, animalInt2);

	// false positive
	compare_between_same_scope(personInt1, animalInt1);
	compare_between_different_scope(personInt1, personInt2);

	return 0;
}
```

[1]:https://www.geeksforgeeks.org/structure-member-alignment-padding-and-data-packing/
