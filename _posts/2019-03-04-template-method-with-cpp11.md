---
layout: post
title: Template Method with Modern c++
tags: [c++]
---

# Hollywood Principle
템플릿 메소드는 "할리우드 원칙"이라는 역전된 제어 구조를 끌어낸다. 할리우드 원칙이란 할리우드에서 오디션을 보고 거절 할 때 주로 쓰던 말로 "전화하지 마세요. 우리가 연락 할게요(Don't call us, we'll call you.)" 상대방의 연락을 차단하는 의미이다. (Design Patterns, GoF).

---

# Template Method Pattern
템플릿 메소드 패턴은 변경 되지 않는 멤버 함수들의 구조는 상위 클래스에서 정의하고, 변경 될 수 있는 멤버 함수의 구현을 하위 클래스로 위임(Deferring)시키는 패턴이다.  

*C++에서는 하나 이상의 순수 가상 함수(Pure Virtual Function)을 가지고 있는 클래스를 추상 클래스(Abstract Class)라 한다.*  

대부분의 추상 클래스에서 사용 할 수 있을 정도로 필수적이고 기본적인 디자인 패턴이다. 또한, 의존 관계 역전 원칙(DIP, Dependency Inversion Principle)을 지킬 수 있는 패턴이므로 깊이 생각 해 볼 필요가 있다.  

**DIP 원칙을 따르기에 하위 모듈의 변경이 상위 모듈에 영향을 미치지 않는 구조를 완성 할 수 있다.**

담고 있는 의미에 비해 구조와 구현은 간단하다.

---

# Template Method Structure
템플릿 메소드 구조는 아래와 같다. 추상 클래스는 TemplateMethod를 정의하고 구현까지 담당한다. TemplateMethod에서는 하위 클래스에서 구현해야 하는 함수들을 호출 한다. 이 함수는 순수 가상 함수나 훅 연산이 될 수 있다.

![Class Diagram](https://upload.wikimedia.org/wikipedia/commons/thumb/5/52/Template_Method_UML.svg/800px-Template_Method_UML.svg.png)


### Hook Operation
훅 연산이란 하위 클래스에서 확장 가능하도록 제공하는 함수로, 보통 상위 클래스에선 구현부가 없이 틀만 제공한다.

# Hook Operation vs Pure Virtual Function
템플릿 메소드 패턴에서는 템플릿 메소드에서 호출하는 연산이 훅 연산인지, 추상 연산인지 정하는 것이 중요하다. 훅 연산은 오버라이드가 가능함을 나타내고, 추상 연산은 꼭 오버라이드를 해야함을 나타낸다.

---

# final and override
템플릿 메소드 패턴에서 템플릿 메소드는 하위 클래스에서 재정의 하지 않아야 한다.
모던 C++ 이 전에는 하위 클래스에서 멤버 함수를 오버라이드 하지 못하게 하기 위해 비가상함수와 접근 제한 방법을 사용 하였다(public, protected, private).  

**모던 C++에서는 [final](https://en.cppreference.com/w/cpp/language/final) 키워드를 사용하여 명시적으로 하위 클래스에서 재정의 하지 못하도록 할 수 있다.**  

또한 하위 클래스에서 재정의 할 수 있는 추상 연산과 훅 연산은 [override](https://en.cppreference.com/w/cpp/language/override) 키워드를 사용하여 잘못 재정의 했을 시의 오류를 방지 할 수 있다.


--- 

# Template Method Pattern Implementation Tatic
1. 템플릿 메소드(final speicifier)를 가지는 상위 클래스를 정의한다.
2. 템플릿 메소드에서 호출 할 멤버 번수를 추산 연산, 훅 연산으로 구분한다.
3. 하위 클래스에서 추상 연산은 반드시 오버라이드 하고, 훅 연산은 필요시 오버라이드 한다.

---

# Abstract Class
위 조건(1~2)에 따른 AbstractClass는 아래와 같다.
```cpp
class AbstractClass {
public:
	virtual void templateMethod() final
	{
		std::cout << "Super: template method." << std::endl;

		this->abstractOperation1();
		this->abstractOperation2();
		this->hookOperation();
	}

protected:
	virtual void abstractOperation1() = 0;
	virtual void abstractOperation2() = 0;

	virtual void hookOperation()
	{
		std::cout << "Super: hook operation." << std::endl;
	}
};
```

# Sub Class
3번 조건에 따른 SubClass 2개는 아래와 같다. SubClass2에는 훅 연산까지 오버라이드 한다.
```cpp
class SubClass1 : public AbstractClass {
protected:
	virtual void abstractOperation1() override
	{
		std::cout << "Sub: abstract operation 1." << std::endl;
	}

	virtual void abstractOperation2() override
	{
		std::cout << "Sub: abstract operation 2." << std::endl;
	}
};

class SubClass2 : public AbstractClass {
protected:
	virtual void abstractOperation1() override
	{
		std::cout << "Sub2: abstract operation 1." << std::endl;
	}

	virtual void abstractOperation2() override
	{
		std::cout << "Sub2: abstract operation 2." << std::endl;
	}

	virtual void hookOperation()
	{
		std::cout << "Sub2: hook operation." << std::endl;
	}
};
```

# Testing
아래는 테스트를 위한 코드이다. 하위 클래스에서 재정의한 연산들이 상위 클래스의 템플릿 메소드에 의해 호출 된다.
```cpp
// g++ main.cpp template-method.cpp -std=c++11
#include <iostream>
#include <memory>

int main()
{
	std::shared_ptr<AbstractClass> obj1 = std::make_shared<SubClass1>();
	obj1->templateMethod();

	std::shared_ptr<AbstractClass> obj2 = std::make_shared<SubClass2>();
	obj2->templateMethod();

	return 0;
}
```

```sh
Super: template method.
Sub: abstract operation 1.
Sub: abstract operation 2.
Super: hook operation.
Super: template method.
Sub2: abstract operation 1.
Sub2: abstract operation 2.
Sub2: hook operation.
```
