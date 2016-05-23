---
layout: post
title:  CMake 입문하기
categories: cmake 
---

 `CMake`에 대한 설명은 생략하고, step-by-step으로 `Cmake`를 익혀보자.  
해당 글은 [Cmake 공식 홈페이지][cmake-tutorial]을 참조하였다.  

**Step 1. 실행파일 만들기**  
프로젝트 : 숫자 두개를 입력받아 덧셈을 출력해주는 `Adder` Program  
아래 두 파일을 같은 위치에 놓고 실행한다.

```
#include <iostream>

using namespace std;

int main (int argc, char *argv[])
{
        if (argc != 3) {
                cout << "Usage : input two numbers." << endl;
                return -1;
        }

        int operatorA = atoi(argv[1]);
        int operatorB = atoi(argv[2]);
        cout << "result :  " << operatorA + operatorB << endl;

        return 0;
}
```

```
# CMakeLists.txt
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (Adder)

ADD_EXECUTABLE(Adder main.cpp)
```

이후 실행  
```
$ cmake.
$ make
$ ./Adder 1 2
``` 

[cmake-tutorial]:https://cmake.org/cmake-tutorial/ 
