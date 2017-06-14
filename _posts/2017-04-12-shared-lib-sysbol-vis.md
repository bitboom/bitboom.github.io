---
layout: post
title:  Shared library symbol visibility
tags: [linux]
---

### [Symbol table][1]  
함수나 변수를 참조, 호출할 때 함수명, 변수명(식별자, identifier)을 사용하고,  
실제 함수, 변수는 메모리(함수: .text region, 변수: .data .stack)에 저장되어 있다.  

이 때 함수명, 변수명을 Symbol이라 하며 컴파일러(or 인터프리터)는 Symbol과 Symbol이 저장되어 있는  
주소를 mapping 하는 Symbol table(hash table)을 만든다.  

컴파일의 결과인 각 목적 파일은 **외부로 보여지는** 식별자들의 Symbol table을 포함 할 것이고,  
링커는 여러 목적 파일을 링킹하는 동안 각 Symbol table을 사용하여 resolve 되지 않은 참조 들을 reslove 한다.

### [Visibility][2]
ELF는 프로그램의 Symbol을 관리하는데 있어서 visibility를 조정 할 수 있게 한다.  
Visibility는 4가지 속성을 가지고 있다.
1. defualt
2. hidden
3. protected
4. internal

> Put simply, it hides most of the ELF symbols which would have previously  
> (and unnecessarily) been public. This means:  
>
> - It very substantially improves load times of your DSO (Dynamic Shared Object).  
> - It reduces the size of your DSO by 5-20%.  
> - Much lower chance of symbol collision.  

즉, Shared library 제작시 외부(library client)에 공개 될 필요 없는 symbol을 숨겨서 size, collision을 줄일 수 있는 기법이다.  
예로 아래와 같은 프로그램에서 common 함수는 외부 파일에 의해 참조 되기 때문에 extern을 사용하였고  
Visibility는 GLOBAL 잡힐 것이다. 

```cpp
// file1.cpp
void common(void) { }  

// file2.cpp
extern void common(void);  
  
void func(void) {  
     common();  
}  
```
```sh
$ gcc -fvisibility=hidden -shared -fPIC  -o library file1.c file2.c
$ readelf --syms ./library
Symbol table '.symtab' contains 55 entries:
   Num:    Value  Size Type    Bind   Vis      Ndx Name
    48: 000003cc     5 FUNC    LOCAL  HIDDEN   12 common
    54: 000003d4    29 FUNC    GLOBAL DEFAULT  12 func
```
---  
common은 외부에 노출할 필요가 없기 때문에 필요한 함수만 Visibility를 셋팅해주면 된다.

```cpp
// file1.cpp
void common_but_not_part_of_api(void) { }  

// file2.cpp
extern void common_but_not_part_of_api(void);  
  
void  __attribute__((visibility("default"))) api_function(void) {  
      common_but_not_part_of_api();  
}
```
```sh
$ gcc -fvisibility=hidden -shared -fPIC  -o library file1.c file2.c
$ readelf --syms ./library
Symbol table '.symtab' contains 55 entries:
   Num:    Value  Size Type    Bind   Vis      Ndx Name
    48: 000003cc     5 FUNC    LOCAL  HIDDEN   12 common
    54: 000003d4    29 FUNC    GLOBAL DEFAULT  12 func
```

- http://codecat.tistory.com/entry/visibility
- http://egloos.zum.com/studyfoss/v/5257309

[1]:https://ko.wikipedia.org/wiki/%EC%8B%AC%EB%B3%BC_%ED%85%8C%EC%9D%B4%EB%B8%94  
[2]:https://gcc.gnu.org/wiki/Visibility
