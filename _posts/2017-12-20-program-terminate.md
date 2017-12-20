---
layout: post
title: exit(), abort(), assert()
tags: [c++]
---

## 프로그램의 종료에 관하여

C++로 작성된 프로그램은 main 함수의 호출이 끝나면 프로그램 프로그램이 종료된다.
main 함수가 끝나기 전 실행흐름에 따라 프로그램을 정상적 비정상적으로 종료할 수 있는 방법을 알아보자.

### exit()
프로세스를 정상적으로 종료하면서, 사용한 리소스를 정리한다.

- thread storage에 저장된 객체가 소멸된다. (C++11)
- static storage에 저장된 객체들이 소멸되며, atexit에 등록된 함수들이 호출 된다.
- 모든 C stream(<cstdio>에 의해 호출된)들이 닫히고 (buffer의 경우 flushed) 생성되어 있는 모든 임시 파일들이 제거된다.
- 프로그램으로 넘어갔던 제어권이 호스트 환경으로 돌아간다.

### abort()
exit()와 다르게 리소스들을 정리하지 않고, atexit()에 등록된 함수들도 호출 하지 않는다. 
호출된 즉시 SIGABRT를 발생시키고, SIGABRT를 처리할 핸들러가 등록되어 있지 않으면 비정상 종료된다. 
아래는 sigaction을 통하여 SIGABRT를 잡는 예제이다.

```cpp
// g++ main.cpp -std=c++11 

#include <iostream>
#include <csignal> 
#include <cstdlib>

int main()
{
	auto abrtHandler = [](int signum) {
		if (SIGABRT == signum)
			std::cout << "ABRT.: " << signum << std::endl;
	};

	struct sigaction intsig;
	intsig.sa_handler = abrtHandler;
	intsig.sa_flags = 0;

	if (sigaction(SIGABRT, &intsig, nullptr) == -1)
		return -1;

	abort();

  std::cout << "Not printed." << std::endl;

	return 0;
}
```
### assert()
파라미터로 들어온 표현식이 거짓이면 abort()를 호출한다. 만약 NDEBUG가 정의 되어 있다면 
프로그램을 abort()를 호출하거나 프로그램을 종료하지 않고 실행흐름을 이어나간다.

```cpp
#include <iostream>
#include <csignal> 
#include <cassert>

int main()
{
	auto abrtHandler = [](int signum) {
		if (SIGABRT == signum)
			std::cout << "ABRT.: " << signum << std::endl;
	};

	struct sigaction intsig;
	intsig.sa_handler = abrtHandler;
	intsig.sa_flags = 0;

	if (sigaction(SIGABRT, &intsig, nullptr) == -1)
		return -1;

	assert(false);

	std::cout << "Not printed." << std::endl;

	return 0;
}
```

```sh
$ g++ main.cpp -std=c++11 -D NDEBUG
$ ./a.out 
Not printed.

$ g++ main.cpp -std=c++11
$ ./a.out 
a.out: main.cpp:19: int main(): Assertion `false' failed.
ABRT.: 6
Aborted (core dumped)
```

http://www.cplusplus.com/reference/cstdlib/exit/  
http://www.geeksforgeeks.org/understanding-exit-abort-and-assert/

