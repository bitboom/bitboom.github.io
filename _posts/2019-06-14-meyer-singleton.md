---
layout: post
title: Meyer’s Singleton
tags: [c++]
---

---

# [Meyer’s Singleton](http://laristra.github.io/flecsi/src/developer-guide/patterns/meyers_singleton.html)
본 글에서는 C++에서 Singleton을 구현하는 방법 중 하나인 Meyer's Singleton 기법에 대해 소개한다.

Scott Meyer에 의해 제안 된 것으로 function static object을 이용하여 간단하게 Singleton을 구현한다. 먼저 샘플 코드를 보면 아래이 상당히 심플하다.

## [Sample code](http://laristra.github.io/flecsi/src/developer-guide/patterns/meyers_singleton.html)
```cpp
struct singleton_t
{
  static singleton_t &instance()
  {
    static singleton_t s;
    return s;
  } // instance
  singleton_t(const singleton_t &) = delete;
  singleton_t & operator = (const singleton_t &) = delete;

private:
  singleton_t() {}
  ~singleton_t() {}
}; // struct singleton_t
```

# Meyer’s Singleton Features
	- Simple code
	- Lazy initialization
	- Thread safety

# How it works?
Meyer’s Singleton를 완전히 이해하려면 아래의 내용에 대해 답할 수 있어야 한다.  

	1. static variable은 thread-safe한가?
	2. Destructor는 private절에 위치 할 수 있는가?

--- 

# Thread-safed static variable

**C++11부터는 static variable에 대해 thread-safe를 보장한다.**  
즉, C++03까지는 thread-safe하지 않았기에, lock과 같은 방법을 이용하여 Singleton을 구현하였다.

```cpp
void foo()
{
	static bool isThreadSafe;
}
```

### Reference
> [section 6.7](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3337.pdf)
> such a variable is initialized the first time control passes through its declaration; such a variable is considered initialized upon the completion of its initialization. [...] If control enters the declaration concurrently while the variable is being initialized, the concurrent execution shall wait for completion of the initialization.  
> 
> The implementation must not introduce any deadlock around execution of the initializer.  
> 
> **If control enters the declaration concurrently while the variable is being initialized, the concurrent execution shall wait for completion of the initialization.**

---

# Private Destructor
생성자가 private에 위치 할 수 있는지는, Singleton의 object의 scope에 따라 다르다.

## Global scope
Singleton의 object가 global scope일 경우 public destuctor를 가져야만 한다. Global scope에선 public member에만 접근 가능하기 때문이다.
```cpp
class A{
public:
	~A() = default;
	static A& instance() {
		return global;
	};
	static A global;
};
```

---

## Class scope
**Singleton object가 class scope일 경우 private destuctor를 가질 수 있다.** 아래와 같이 class내에서 static function object로 선언 된 다면 lazy initialization가 적용되어, 함수 최초 호출 시에 초기화 되고 프로그램이 종료 될 때 소멸 된다. (프로그램이 종료 될때 class scope에서 destructor를 호출 할 수 있기 때문에 가능하다.)
```cpp
class A{
public:
	static A& instance() {
		static A local;
		return local;
	};
private:
	~A() = default;
};
```

### Effect
Private destructor를 활용 하면 Singleton object가 강제 'delete'되는 것을 방지 할 수 있다. (함수가 포인터를 리턴할 때)
```cpp
A* a = A::instance();
delete a; // Error
```


# Caution
*Data members간에 thread safety문제가 있을 수 있는데, data members를 여러개 사용할 땐 더 알아보고 적용해야한다.*

# References
- http://laristra.github.io/flecsi/src/developer-guide/patterns/meyers_singleton.html
- https://stackoverflow.com/questions/2559750/private-destructor-for-singleton-class
