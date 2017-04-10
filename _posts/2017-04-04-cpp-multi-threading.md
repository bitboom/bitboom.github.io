---
layout: post
title: C++ Multithreading Programming
tags: [c++]
---

### Thread  
Disk에 저장되어 실행가능한 파일로 되어 있는 것을 프로그램이라 하며, 해당 프로그램이  
운영체제로부터 자원(CPU, Virtual memory...)을 할당 받아 실행 중인 동적인 객체를 프로세스라 한다.  

프로세스가 수행되려면 자원(Resource)와 수행 흐름(Flow of control)이 필요한데, 일반적인 상식으론
이 수행 흐름을 Thread라 하며 한 프로세스 내 여러 Thread가 동시에 수행하는 것을 MultiThreading이라 한다.  
Linux에선 프로세스와 Thread를 모두 task의 개념으로 보고 실제 parent task와 자원을 어떻게 공유하고  
접근 제어하느냐에 따라서 프로세스와 Thread로 구분한다.

### [Multithreading][1]
> Multithreading is mainly found in multitasking operating systems.  
> Multithreading is a widespread programming and execution model  
> that allows multiple threads to exist within the context of one process.  
> *These threads share the process's resources, but are able to execute independently*.  
> The threaded programming model provides developers with a useful abstraction of *concurrent execution*.  
> Multithreading can also be applied to one process to enable parallel execution  
> on a multiprocessing system.  

### [Mutex][2]  
Multithreading 중 각 thread가 실행 되기 위해선 운영체제로부터 CPU를 할당을 받아야 한다.  
이때 발생할 수 있는 race coditions을 방지하기 위해 concurrency control을 하여야 하는데  
그 중 대표적인 것이 mutex이다. 실행(running)될 Thread는 critical section에 들어가기 전에  
mutex를 들고 lock상태로 진입한다. 다른 thread는 해당 Thread가 lock을 해제하고 mutex를 놓을 때까지  
wait 상태로 머문다. 즉, critical section에 진입하기 위해 mutex를 해제하고 signal을 기다리다가  
signal이 도착하면 mutex를 잡고 다음 코드를 실행한다.

### Condition Variable (event)  
Thread간 signal을 교환하거나 특정 memory buffer에서 값을 읽어야 할 때 condition variable을 사용한다.  
Memory buffer에서 값을 읽어 올 때 lock을 얻기까지 계속 기다리면 busy waiting이 되나  
Condition Variable을 사용하면 이를 피할 수 있다. 예로 Thread A에서 input을 수행하고 Thread B에서 output을  
수행할 때 codition variable을 써서 Thread A에서 input 수행이 끝난 후 Thread B에게 signal을 보내면 된다.

### [std::thread][3]  
기존 Linux에선 [POSIX Multithreading][4]로 Multithreading을 수행하였지만 C++ 11부터 std::thread를 이용한  
Multithreading이 가능하게 되었다.  

# Code Multithreading with c++11

### 1. Multithreading with mutex
```cpp
// g++ main.cpp -std=c++11 -pthread

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

// lock guard - RAII pattern of mutex
#define __BEGIN_CRITICAL_SECTION__ { std::unique_lock<std::mutex> lock(m);
#define __END_CRITICAL_SECTION__ }

// synchronization primitive
std::mutex m;

int sharedData = 0;

int main()
{
	std::vector<std::thread> workers;
	for (int i = 0; i < 5; i++) {
		// generate thread
		workers.emplace_back([]() {
			std::thread::id id = std::this_thread::get_id();

			for (int i = 0; i < 5; i++) {
				__BEGIN_CRITICAL_SECTION__
				// std::cout stream : shared resource
				std::cout << "[" << id << "] -> " << sharedData++ << std::endl;
				std::this_thread::sleep_for(std::chrono::seconds(1));
				__END_CRITICAL_SECTION__
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		});
	}

	for (std::thread &worker: workers) {
		if (worker.joinable()) {
			worker.join();
		}
	}
}
```

### 2. Multithreading with condition variable
```cpp
// g++ main.cpp -std=c++11 -pthread

#include <iostream>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <vector>

// lock guard - RAII pattern of mutex
#define __BEGIN_CRITICAL_SECTION__ { std::unique_lock<std::mutex> lock(m); \
									 cv.wait(lock);
#define __END_CRITICAL_SECTION__ } cv.notify_one();

// synchronization primitive
std::mutex m;
std::condition_variable cv;

int sharedData = 0;

int main()
{
	std::vector<std::thread> workers;
	for (int i = 0; i < 5; i++) {
		// generate thread
		workers.emplace_back([]() {
			__BEGIN_CRITICAL_SECTION__
			std::thread::id id = std::this_thread::get_id();
			for (int i = 0; i < 5; i++) {
				// std::cout stream : shared resource
				std::cout << "[" << id << "] -> " << sharedData++ << std::endl;
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
			__END_CRITICAL_SECTION__
		});
	}

	// careful! Shouldn't notify() before wait()
	std::this_thread::sleep_for(std::chrono::seconds(1));
	cv.notify_one();
	for (std::thread &worker: workers) {
		if (worker.joinable()) {
			worker.join();
		}
	}
}
```


[1]:https://en.wikipedia.org/wiki/Thread_(computing)#Multithreading  
[2]:https://en.wikipedia.org/wiki/Mutual_exclusion  
[3]:http://en.cppreference.com/w/cpp/thread/thread
[4]:http://www.cs.kent.edu/~ruttan/sysprog/lectures/multi-thread/multi-thread.html
