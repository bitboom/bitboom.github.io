---
layout: post
title:  Pimpl Idiom
tags: [c++] 
---

[MSDN]의 `Pimpl(Pointer to Implementation) Idiom`의 정의는 아래와 같다. 
 
 > The pimpl idiom is a modern C++ technique to **hide implementation**,  
 > to minimize coupling, and to separate interfaces.  
 > Pimpl is short for "pointer to implementation."  
 > You may already be familiar with the concept but know it by other names  
 > like Cheshire Cat or Compiler Firewall idiom.  
 
구현부를 숨겨서 coupling을 최소화하고, interfaces를 분리한다.  
예를 들어, library를 제공하기 위한 header를 제공한다 했을 때,  
1. library user는 header 안에 선언된 모든 header를 참조하게 되고
2. header안에 제공하는 struct와 같은 것이 변경되면 re-complie을 해야한다.

위와 같은 문제를 해결하기 위하여 도입된 Idiom이며 크게 장점은 아래와 같다.  

 > Minimization of compilation dependencies.  
 > Separation of interface and implementation.  
 > Improve potablity.  
 > Compile time capsulation.  

**즉, code간 coupling을 줄이고 complie의 효율성을 증가** 시킬 수 있다.  
아래 pimpl idiom을 적용하기 전 code와 후 code를 작성 하였다.    
header가 매우 깔끔해진 것을 볼 수 있다.

단 기존 C98로 구현 시 Impl 객체를 new, delete하여 구현 하면 되지만,  
**modern c++**로 구현시는 implemet type 및 move semantics에 따라  
고려해야 할 부분이 있다. 아래코드에 반영 되어있다.   

```
// non-pimpl.h
#include <string>
#include "audit/log.h"

class NonPimpl {
public:
	NonPimpl();
	virtual ~NonPimpl();
	void task(void);
private:
	std::string m_title;	
};

// non-pimpl.cpp
#include "non-pimpl.h"

NonPimpl::NonPimpl() : m_title("non-pimpl") {}
NonPimpl::~NonPimpl() {}
void NonPimpl::task() {}
``` 

```
// pimpl.h
class Pimpl {
public:
	Pimpl();
	virtual ~Pimpl();
	Pimpl(const Pimpl&& rhs);
	Pimpl& operator=(const Pimpl&& rhs);
private:
	class Impl;
	std::unique_ptr<Impl> m_impl;	
};

// pimpl.cpp
#include "pimpl.h"
#include <string>
#include "audit/log.h"

class Pimpl::Impl {
public:
	Impl() : m_title("pimpl") {}
	virtual ~Impl() {}
	void task() {}
private:
	std::string m_title;
};

Pimpl::Pimpl() : m_impl(new Impl) {}
Pimpl::~Pimpl() = default;
Pimpl::Pimpl(const Pimpl& ths) : m_impl(nullptr)
{
	if (rhs.m_impl) m_impl = std::make_unique<Impl>(*rhs.m_impl);
}
Pimpl& Pimpl::operator=(const Pimpl& rhs)
{
	if (!rhs.m_impl) m_impl.reset();
	else if (!m_impl) m_impl = std::make_unique<Impl>(*rhs.m_impl);
	else *m_impl = *rhs.m_impl;
	return *this;
}
void Pimpl::task() { return m_impl->task(); }
``` 

[MSDN]:https://msdn.microsoft.com/ko-kr/library/hh438477.aspx 
