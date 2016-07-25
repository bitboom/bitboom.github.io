---
layout: post
title:  C++ coding style about char *
tags: [c++] 
---

string의 memory를 할당 한 후 char *를 반환해주는 특정 함수를  
unique_ptr<char>로 다룰 수 있다.

만약 char *로 받는다면 다시 free()를 명시적으로 호출 해주어야 하지만  
unique_ptr로 다룬다면 아래와 같은 code로 깔끔하게 작성 할 수 있다.  

```
// c style
    char *str = get_new_str_func();
    ..
    free(str);
// c++ style
    std::unique_ptr<char> str(getNewStrFunc());
``` 

만약 size를 알고 있거나 넉넉하게 잡을 수 있는(다시 resize 가능)하다면  
vector를 이용하여 구현 할 수도 있다.

```
// c style
    char *str = (char *)malloc(sizeof(char) * STR_SIZE);
    snprintf(str, sizeof(char) * STR_SIZE, format, arg);
    free(str);
// c++ style
    std::vector<char> buf(STR_SIZE, '\0');
    snprintf(buf.data(), buf.size(), format, arg);
``` 
