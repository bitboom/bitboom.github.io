---
layout: post
title: Run-time type information and dynamic_cast
tags: [c++] 
---

### [RTTI(런타임 타입 정보)][1]
RTTI는 런타임시에 객체의 타입을 확인하는 C++의 메커니즘이다. 대표적으로 dynamic_cast, typeid를 들 수 있다. 이를 이해하기 위해선 먼저 dynamic_cast와 static_cast의 차이를 이해하여야 한다.

- C++ 버전의 [Type introspection][2] 개념이다.

### dynamic_cast vs static_cast
C++ 스타일의 casting 방식은 4가지로 모든 타입 변환이 가능했던 C 스타일의 캐스팅에서 상황에 
맞게 사용하도록 고안 되었다.
- static_cast
- dynamic_cast
- const_cast
- reinterpret_cast

여기서 static_cast와 dynamnic_cast의 가장 큰 차이점은 타입을 결정하는 시기인데 static_cast의 경우 컴파일 타임에 타입을 결정한다. 반대로 dynamic_cast의 경우 런타임에 타입을 결정한다.  

dynamic_cast의 경우 다형성을 가지는(virtual function) 상속 관계에서의 형변환에 사용하도록 고안 되었고 주로 down-casting시에 사용된다. 다형성을 가지지 않는 상속 관계의 형변환시 사용하였을 때
컴파일 에러를 발생시킨다.

static_cast의 경우 기본적인 C스타일의 캐스팅과 같은 방식의 연산자다. static의 의미는 정적 형변환 
즉, 컴파일 타임에 형변환이 진행 된다는 의미이다.  

이 또한 상속 관계에서 사용 될 수 있는데 정확한 사용 시점을 확인하면 아래와 같다.

Person은 virtual member function으로 eat()를 가지고 있고, 다형성 원칙에 의해 Person을 상속 받는
모든 객체에 대해 형변환 후 eat()을 호출 할 수 있다. Student든 Doctor든 정적으로 형변환을 진행하기에 정확히 어떤 타입으로 형변환 되는지 알고 있을 때 static_cast를 사용한다.  

```cpp
class Person;
class Student : public Person;
class Doctor : public Person; 

auto student = static_cast<Student*>(person);
student.eat();

auto doctor = static_cast<Doctor*>(person);
if (doctor != nullptr)
	doctor.surgery();
```

반대로 dynamic_cast의 동적으로 형변환을 시도, 질의 한다는 것인데 **Person Class를 상속 받은 derived class 중에 Doctor class라면** surgery() 수술을 진행하라 라는 것이다. 만약 형변환이 
실패 하였을 경우 nullptr을 리턴하며 이를 통해 확인 할 수 있다.  

만약 위와 같은 상황에서 Student로 casting 하였을 경우 Student 또한 Peson에 derived class이기 
때문에 형변환이 일어나고 슬라이싱 문제가 발생 한다.

### 결론
따라서 다형성 관계에서 형변환을 시도하는 객체가 정확히 어떠한 타입으로 형변환이 이루어 지는지 
알고 있을때 static_cast를 사용한다.  
반대로 런타임에 타입을 확인해서 수행해야 할때 dynamic_cast를 사용한다. (형변환이 실패 되면 
nullptr를 리턴하기에 확인 가능)

### etc
dynamic_cast를 사용하려면 (정확히 down-casting으로) 컴파일시 rtti 옵션이 커져있어야 한다.
하지만 dynamic_cast는 아래의 문제를 가지고 있다.
 - static_cast에 비해 비용이 비싸다.
 - 시스템에 따라 차이가 있는 결과를 나타낸다.

위와 같은 문제로 dynamic_cast, 나아가 rtti를 사용하지 않도록 하려면 컴파일시 옵션(-fno-rtti)을 주어야 하는데 이 때 주의할 점이 있다.  


 - rtti가 꺼져있는 라이브러리
 - rtti가 켜져있는 바이너리 (downcasting으로 dynamic_cast를 사용)

위 두개를 링킹 하였을 때 "undefined-reference-to-typeinfo"의 에러를 만날 수 있다.


### reference
https://stackoverflow.com/questions/307352/g-undefined-reference-to-typeinfo
http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0491c/CHDGGBAI.html
http://sweeper.egloos.com/m/1907485
http://web.archive.org/web/20100503172629/http://www.pubbs.net/201004/gcc/25970-linker-error-undefined-reference-to-typeinfo-for-a-with-no-rtti-option.html

[1]:https://en.wikipedia.org/wiki/Run-time_type_information
[2]:https://en.wikipedia.org/wiki/Type_introspection
