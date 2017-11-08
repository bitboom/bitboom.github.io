---
layout: post
title:  가상 소멸자에 관하여
tags: [c++]
---

C++ 프로그래밍 중 클래스를 정의하면서 상투적으로 사용하고 있지만, 정확한 이해를 위하여 가상 소멸자에 대하 정리하여 본다.  

우선 가상 소멸자를 이해하려면 두가지 용어(다형성, 가상 함수)을 알고 있어야 한다.

### [다형성 (Polymorphism)][1] 

> Polymorphism (from Greek πολύς, polys, "many, much" and μορφή, morphē, "form, shape")  
> Polymorphism is the **provision** of a single interface to entities of different types.

다형성은 그리스어의 "여러 + 형태"를 뜻하는 에서 유래된 단어로 객체 지향 프로그래밍에서 여러 타입의 객체들을 단일의 인터페이스로 규정 할 수 있는 것을 뜻한다.  

즉, 각기 다른 객체들의 행동을 동일한 방식으로 표현 할 수 있는 것이다.  
가상 함수는 이를 가능할 수 있도록 해준다.

### [가상 함수 (Virtual Function)][2]
**가상 함수**는 상속하는 클래스 내에 오버라이딩 될 수 있는 함수를 이야기 한다.  
이때 상위 클래스에서는 가상 함수를 선언만 하고 구현 부분을 하위 클래스에 위임하는 것을 순수 가상 함수(Pure Virtual Funcion)라 하며 순수 가상 함수를 가지고 있는 클래스를 추상 클래스(Abstract Class)라 한다.  

예를 들어 동물이라는 추상 클래스(Abstract class)에 말을 하다의 행동을 순수 가상 함수(Pure virtual funcion)으로 정의한다고 하자.  
이를 상속 받는 강아지와 고양이의 클래스를 정의하였을 때, listening이라는 함수에서는 강아지와 고양이를 구분 할 필요 없이, 동물이 말을 하다라는 하나의 인터페이스로 규정 할 수 있다.  

```cpp
#include <iostream>
#include <string>

class Animal {
public:
	virtual std::string talk(void) = 0;
};

class Dog : public Animal {
public:
	virtual std::string talk(void) override
	{
		return "Woof!";
	}
};

class Cat : public Animal {
public:
	virtual std::string talk(void) override
	{
		return "Meow!";
	}
};

void listening(Animal* animal)
{
	std::cout << animal->talk() << std::endl;
}

int main()
{
	listening(dynamic_cast<Animal*>(new Dog()));
	listening(dynamic_cast<Animal*>(new Cat()));
	return 0;
}

// output:
// Woof!
// Meow!
```

위의 예에서 보듯 listening 함수 에서는 Animal의 객체를 인자로 받지만, 실제 객체(Dog, Cat)의 함수를 호출 한다.  

talk 함수를 만약 순수 가상 함수나 가상 함수로 정의하지 않고 단순 오버라이딩을 하였을 경우, 실제 객체와 상관 없이 Animal 객체의 함수를 할 것이다. 

```cpp
#include <iostream>
#include <string>

class Animal {
public:
	std::string talk(void)
	{
		return "Shouldn't be called.";
	}
};

class Dog : public Animal {
public:
	std::string talk(void)
	{
		return "Woof!";
	}
};

class Cat : public Animal {
public:
	std::string talk(void)
	{
		return "Meow!";
	}
};

void listening(Animal* animal)
{
	std::cout << animal->talk() << std::endl;
}

int main()
{
	listening(dynamic_cast<Animal*>(new Dog()));
	listening(dynamic_cast<Animal*>(new Cat()));
	return 0;
}

// output:
// Shouldn't be called.
// Shouldn't be called.
```

### 가상 소멸자(Virtual Destructor)
**위의 상황에서 추론 할 수 있드시 가상 함수로 선언되지 않으면 실제 객체의 함수가 호출 되지 않는다.**

이를 소멸자에 적용하여 보았을 땐, 실제 객체의 소멸자는 호출되지 않고 해당 객체를 가르키는 포인터 타입의 객체의 소멸자가 호출 됨을 뜻한다.  

소멸자에서 자원 해제의 코드가 들어 있을 땐 메모리 릭과 같은 문제를 유발 할 수 있다.  

결론적으로 상속될 염려가 있는 클래스들은 가상 소멸자를 정의하여야 하고, 그렇지 않을 때는 가상 소멸자를 정의 할 필요가 없다. 다만 이때는 상속을 금지하는 키워드인 [final specifier][3]을 붙여주는 것이 좋다.

[1]:https://en.wikipedia.org/wiki/Polymorphism_(computer_science)
[2]:https://en.wikipedia.org/wiki/Virtual_function
[3]:http://en.cppreference.com/w/cpp/language/final
