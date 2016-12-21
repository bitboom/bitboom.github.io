---
layout: post
title:  exception handiling with object lifetime
tags: [c++] 
---

## [constructor 내 exception throw][1]
- constructor 내에서 exception을 throw 했을 경우, destructor가 호출 되지 않음.
- constructor가 완전히 진행 되어야 object의 lifetime이 시작되기 때문.

## [constructor 밖 exception throw][2]  
- 반대로 constructor가 성공적으로 호출이 되었다면, exception handling이 이루어지는  
  logic 내에서 destructor가 호출 


[1]: http://stackoverflow.com/questions/9971782/destructor-not-invoked-when-an-exception-is-thrown-in-the-constructor
[2]: http://stackoverflow.com/questions/8311457/are-destructors-called-after-a-throw-in-c
