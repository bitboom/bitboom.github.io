---
layout: post
title:  noexcept, throw
tags: [c++] 
---

C++에서는 try-catch 구절을 통해 예외를 처리한다. 함수는 어디서나 예외를 던질 수 있으며, 예외를 처리하지 
않으면 terminate()가 호출 되고 SIGABRT가 발생한다. 함수는 C++ 11 이전에 함수 선언 시 발생시킬 예외를 
throw-list를 통해 기술할 수 있었다. 하지만 throw-list는 아래의 문제점을 지니고 있다.

- 템플릿 메서드의 경우 익셉션을 특정하기 어렵다.

템플릿은 어떤 타입으로 인스턴스화 될 지 모르기 때문에 해당 인스턴스가 어떤 익셉션을 발생 시키는지 예측 할 수 없다. 
또한, throw-list에 기술하지 않은 예외가 발생되면 프로그램은 unexpected()를 호출 하고 이 함수는 디폴트로 
terminate()를 호출하게 된다. 결국 SIGABRT가 발생하게 되는 것이다.

이와 같은 여러가지 문제로 C++11부터는 noexcept를 제외하고는 throw-list 기능이 중단 되었다.

### [noexcept operator (since c++11)][1]

C++11 부터 추가된 keyword로 인터페이스 설계 시 중요한 역할을 한다. 기본적으로 아래의 noThrow()는 
예외를 발생시키지 말아야 한다.
- Client에게 해당 함수가 예외처리가 필요한지 아닌지에 대한 정보를 전달한다. 

```cpp
void noThrow(void) noexcept {}
```

그렇다면 noexcept keyword가 선언 된 함수에서 예외를 발생시키면 어떻게 될까?  
프로그램은 terminate()를 호출하고 결국 SIGABRT가 발생 하게 된다.
절대적으로 noexcept keyword가 선언된 함수에서는 예외를 발생시키면 안된다.

아래의 코드는 noexcept가 선언된 함수에서 예외를 발생시켜 SIGABRT가 발생된 예제이다.
noThrow()에서 예외를 던지고 main()에서 예외를 잡았음에도 SIGABRT가 발생하게 된다.

```cpp
#include <iostream>
#include <csignal> 
#include <stdexcept>

void noThrow(void) noexcept { 
	throw 1;
}

int main()
{
	auto abrtHandler = [](int signum) {
		if (SIGABRT == signum)
			std::cout << "ABRT: " << signum << std::endl;
	};

	struct sigaction abrtAct;
	abrtAct.sa_handler = abrtHandler;
	abrtAct.sa_flags = 0;

	if (sigaction(SIGABRT, &abrtAct, nullptr) == -1)
		return -1;

	try {
		noThrow();
	} catch (int e) {
		std::cout << e << std::endl;
	}

	return 0;
}
```
```sh
$ g++ -std=c++11 main.cpp 
$ ./a.out 
terminate called after throwing an instance of 'int'
ABRT: 6
Aborted (core dumped)
```

### [non-throwing function (since c++11)][2]  
기본적으로 예외를 던지지 않는 함수들  
- destructor
- deallocation function
- ...

소멸자의 경우 default로 noexcept가 선언되어 있으며 이를 무시하고 
소멸자에서 예외를 던져야 한다면 명시적으로 noexcept(false)를 선언해주어야 한다. 그렇지 않으면 
예외를 처리한다해도 SIGABRT가 발생되며 프로그램이 종료된다.

```cpp
#include <iostream>
#include <stdexcept>

class NoThrow {
public:
	NoThrow() = default;
	~NoThrow() noexcept(false) {
		throw 1;
	};
};

int main()
{
	try {
		NoThrow n;
	} catch (int e) {
		std::cout << e << std::endl;
	}

	return 0;
}
```

[1]: http://en.cppreference.com/w/cpp/language/noexcept
[2]: http://en.cppreference.com/w/cpp/language/noexcept_spec
