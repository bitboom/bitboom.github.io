---
layout: post
title: The rule of five/zero
tags: [c++]
---

C++11부터 이동 생성자, 이동 대입 연산자를 지원함에 따라 컴파일러가 자동으로 생성해 주는 함수(Compiler-generated Functions, 이하 CGF)들이 늘어 났다. 본 글에서는 5가지의 CGF를 작성하는 규칙에 관한 내용이다.

---

5-CGF
- destructor
- copy constructor
- move constructor
- copy assignment operator
- move assignment operator

---

컴파일러는 프로그래머가 5-CGF를 명시적으로 작성하지 않아도 자동으로 생성해주지만, 예외가 있다.
- 소멸자가 명시적으로 작성되면, 이동 생성자/대입연산자를 자동으로 생성해 주지 않는다.
	- **소멸자만 작성할 경우 정상 작동하지만 이동 시맨틱을 사용하지 않게 되는 큰 Loss가 발생한다.**
	- Base class로 사용하기 위하여 가상 소멸자를 선언하는 경우도 역시 포함 한다.

---

# [The rule of five/zero](https://en.cppreference.com/w/cpp/language/rule_of_three)

Martinho Fernandes와 Scott Meyers는 자신의 블로그에 the rule of zero와 [the rule of five default](http://scottmeyers.blogspot.com/2014/03/a-concern-about-rule-of-zero.html)를 각각 제안 하였다.

## The Rule of Zero
> Rule of Zero by Martinho Fernandes: Classes that have custom destructors, copy/move constructors or copy/move assignment operators should deal exclusively with ownership. Other classes should not have custom destructors, copy/move constructors or copy/move assignment operators

핵심은 모든 클래스를 아래 둘 중 하나로 작성하라는 것이다.

1. 5-CGF를 모두 작성한 클래스  

```cpp
class X {
public:
  ~X() = { /// };
  X(const X&) = default;
  X(X&&) = default;
  X& operator=(const X&) = default;
  X& operator=(X&&) = default;
  /// member functions
};
```

2. 5-CGF를 모두 작성하지 않은 클래스.
```cpp
class X {
  /// member functions
};
```

---

## The Rule of Five Default

> Rule of the Five Defaults: Classes that declare custom destructors, copy/move constructors or copy/move assignment operators should deal exclusively with ownership. All other classes should explicitly default their destructors, copy/move constructors and copy/move assignment operators.

이 규칙의 핵심은x모든 클래스의 5-CGF를 구현 되어야 한다는 것이다. 구현이 필요 없는 경우(trivial)도 "= default"로 작성 하라는 것이다. (디버그용이나 나중에 5-CGF 중 하나를 구현하게 되었을 때를 대비)  

```cpp
class X {
public:
  ~X() = { /// };
  X(const X&) = default;
  X(X&&) = default;
  X& operator=(const X&) = default;
  X& operator=(X&&) = default;
  /// member functions
};
```

이 외에도 비슷한 맥락으로 [The Rule of All or Nothing](https://arne-mertz.de/2015/02/the-rule-of-zero-revisited-the-rule-of-all-or-nothing/)와 같이 각자의 견해로 5-CGF에 대한 규칙을 정하고 있다.

---

# The point
유사한 방식으로 여러가지의 규칙을 정의하고 구현에 관한 가이드라인을 제공한다. 각각의 규칙에는 장단점과 많은 논의가 있다.

하지만 이 규칙들에서 말하고자하는 명백한 것은 "소멸자만 따로 정의함에 있어 컴파일러가 이동 생성자/대입연산자를 자동으로 생성하지 못하게 하는 것을 방지하라."이다.

즉, **이동 시맨틱을 지원하지 않는 C++ 코드를 짜는 우를 범하지 마라.** 는 것
