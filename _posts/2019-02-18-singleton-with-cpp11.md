---
layout: post
title: Singleton with Modern c++
tags: [c++]
---

# Singleton Pattern

싱글턴 패턴(Singleton pattern)을 시스템에서 정확히 하나의 인스턴스만 유지해야 하는 클래스에 적용하는 디자인 패턴이다.  

기존 C++에서의 싱글톤 패턴 구현은 static 키워드로 구현 되어 하나의 인스턴스를 생성하도록 하였다. 하지만 static 키워드로의 구현은 멀티-스레딩환경에서 안전하지 않다. (Non-Thread Safe)

--- 

# Singleton with Modern C++
C++11 부터 [std::once_flag](https://en.cppreference.com/w/cpp/thread/once_flag)를 사용하여 Thread-Safe하게 싱글톤 패턴을 구현할 수 있다.

# Singleton Usage
데이터베이스 커넥션을 위한 클래스나 로그를 찍기 위한 클래스 등에 사용한다. 

# Singleton Implementation Tatic
1. 생성자를 숨기고 인스턴스 게터 함수(Getter)를 제공 한다.
2. 복사 생성자와 이동 생성자를 삭제 한다.
3. std::once_flag를 사용하여 인스턴스 생성 과정을 Thread-safe하게 한다.
4. std::unique_ptr을 사용하여 하나의 인스턴스를 보장 한다.

---

# Singleton Declaration
위 조건(1~4)에 따른 헤더파일 정의부는 아래와 같다.
```cpp
// singleton.hxx
#pragma once

#include <memory>
#include <mutex>

#include <iostream>

class Singleton final {
public:
	~Singleton() = default;

	Singleton(const Singleton &) = delete;
	Singleton(Singleton &&) = delete;
	Singleton &operator=(const Singleton &) = delete;
	Singleton &operator=(Singleton &&) = delete;

	static Singleton& GetInstance(void);

private:
	Singleton() = default;

	static std::unique_ptr<Singleton> instance;
	static std::once_flag flag;
};

```

# Singleton Definition
3번 조건과 디버깅을 위한 구현부는 아래와 같다.
```cpp
// singleton.cpp
#include "singleton.hxx"

std::unique_ptr<Singleton> Singleton::instance = nullptr;
std::once_flag Singleton::flag;

Singleton& Singleton::GetInstance(void)
{
	std::call_once(Singleton::flag, []() {
		std::cout << "Create Instance." << std::endl;
		Singleton::instance.reset(new Singleton);
	});

	std::cout << "Get Instance." << std::endl;
	return *Singleton::instance;
}
```

# Singleton Testing
아래는 테스트를 위한 코드이다. 첫 호출 시에만 인스턴스 생성 코드가 동작하는 것을 확인 할 수 있다.
```cpp
// g++ main.cpp singleton.cpp -std=c++11 -pthread
// main.cpp
#include "singleton.hxx"

int main() try {
	Singleton::GetInstance();
	Singleton::GetInstance();
	Singleton::GetInstance();

	return 0;
} catch (std::exception& e) {
	std::cout << e.what() << std::endl;
}
```

