---
layout: post
title:  c++ static member initialization 주의사항
tags: [c++] 
---

variable의 lifetime을 global하게 가져가기 위해  
C++에서 class member 변수로 또는 일반적으로 static 변수를 사용하는데  
class member 변수로 사용할 경우 header에서 선언(class declaration)하고  
.cpp(member definition or initialization) 에서 초기화를 진행한다.  

이 변수들을 참조하기 전에 주의할 점이 초기화 타임이다.
일반적으로 아래와 같이 static initialization이 먼저 난다.

```
// logger.h

class Logger {
public:
	static void setTag(const std::string& str);
private:
	static std::string sTag;
	static std::string dTag;
};

// logger.cpp
//  static initialization
std::string Logger::sTag = "Default";

// dynamic initialization
std::unique_ptr<std::string> Logger::dTag([]() {
	auto tag = Logger::getTag();
	return tag.empty() ? new std::string("Defualt") : new std::string(tag);

}());

void Logger::setTag(const std::string& str) {
	
}
```
> Formally, C++ initializes such variables in three phases:  
> 1. Zero initialization  
> 2. Static initialization  
> 3. Dynamic initialization
> all static objects (objects with static lifetime) are first zero initialized,  
> then objects with static initialization are initialized,  
> and then dynamic initialization occurs.

# 주의사항  
1. 이 라이브러리 내 sTag가 initialization 되는 시점  
2. 이 라이브러리를 사용하는 client에서 setTag로 initialization 되는 시점  

위 두 경우의 순서를 보장 받을 수 없기에,  
dTag처럼 값이 있으면 그 값을 사용 할 수 있도록 해야한다.  
(2가 먼저 될경우 ex. __attribute__((constructor)) // main함수보다 먼저 실행)  
*이를 하기 위해 dTag는 포인터로 선언되어 있어야 한다. 아니면 segmetation fault*
*선언만 되어있고 그 값을 setTag로 먼저 참조하므로*

# 결론 
1. static member를 포인터로 선언  
2. static member initialization 전 값 확인  

ref : http://stackoverflow.com/questions/17783210/when-are-static-and-global-variables-initialized
