---
layout: post
title: Analyze container_of and offsetof
tags: [kernel]
---

container_of and offsetof are frequently used in kernel code.  

If we declare a structrue variable somewhere and know the pointer of the structure's member, we can access the structure variable by container_of.
```c
/// If we declare a structrue variable somewhere
Foo foo;
...
/// and we know the pointer of the structure's member
int* mptr = &(foo.id);
...
/// we can access the structure variable.
Foo* foo_ptr = container_of(mptr, struct Foo, id);
```

Let's look at an example first.

```c
#include <stdio.h>

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

struct Foo {
	const char* name;
	int id;
};

struct Foo g_foo = { .name = "global foo", .id = 10 };

int main() {
	printf("Foo::name offset is %lu.\n", offsetof(struct Foo, name));
	printf("Foo::id offset is %lu.\n", offsetof(struct Foo, id));

	struct Foo l_foo = { .name = "local foo", .id = 20 };

	int* mptr = &(g_foo.id);
	printf("Foo 's name: %s.\n", container_of(mptr, struct Foo, id)->name);
	printf("Foo 's id: %d.\n", container_of(mptr, struct Foo, id)->id);

	mptr = &(l_foo.id);
	printf("Foo 's name: %s.\n", container_of(mptr, struct Foo, id)->name);
	printf("Foo 's id: %d.\n", container_of(mptr, struct Foo, id)->id);

	return 0;
}
```
```sh
# result
Foo::name offset is 0.
Foo::id offset is 8.
Foo 's name: global foo.
Foo 's id: 10.
Foo 's name: local foo.
Foo 's id: 20.
```

# Code anatomy
To know container_of, we need to know offsetof.

## [offsetof](https://elixir.bootlin.com/linux/latest/source/include/linux/stddef.h#L15)

**offsetof** is defined in <stddef.h> header and it returns the offset of stucture's member.
```cpp 
struct Foo {
	char* a; // offsetof(struct Foo, a) == 0
	int b; // offsetof(struct Foo, b) == 8 (pointer size is 8bytes on 64bit system)
};

Foo foo;
```  
  
| pointer | pointer | address |
|:---:|:---:|:---:|
| &foo | &foo.a | 0x00 |
|  |  | 0x01 |
|  |  | 0x02 |
|  |  | 0x03 |
|  |  | 0x04 |
|  |  | 0x05 |
|  |  | 0x06 |
|  |  | 0x07 |
|  | &foo.b | 0x08 |
  
We can check data-structure-alignment.
```cpp 
struct Bar {
	char a; // offsetof(struct Bar, a) == 0
	int b; // offsetof(struct Bar, b) == 4 (not 1)
};
```

## offsetof - detail
Let's see the detail. It's very fun code.
```c
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
``` 
1. (TYPE * )0: Cast address 0 to Type* (Now Type* points 0x00)
2. ((TYPE * )0)->MEMBER: Access to Member by -> operator
3. &((TYPE * )0)->MEMBER: Get address by & operator.
4. (size_t) &((TYPE * )0)->MEMBER: Cast to (size_t)

---

## [container_of](https://elixir.bootlin.com/linux/v3.19.8/source/include/linux/kernel.h#L798)
The code of container_of is similar to offsetof. This code is on kernel 3.19.

```c
/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})
```
The second line is same method of offsetof.  
Let's see the third line. We can easily get structure address by subtracting offset.  
(&foo == &foo.b - 8)  

| pointer | pointer | offset |
|:---:|:---:|:---:|
| &foo | &foo.a | 0 |
|  |  | 1 |
|  |  | 2 |
|  |  | 3 |
|  |  | 4 |
|  |  | 5 |
|  |  | 6 |
|  |  | 7 |
|  | &foo.b | 8 |
  
Note that "(char * )__ mptr". Since pointer arithmetic depends it's type. "double * + 1" increases 8 bytes not 1 byte.
So, we should cast to (char * ) for increasing 1 byte.

That's all.  

The lastest code of container_of is slightly different. Refer [this](https://elixir.bootlin.com/linux/latest/source/include/linux/kernel.h#L969).
```c
/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 */
#define container_of(ptr, type, member) ({				\
	void *__mptr = (void *)(ptr);					\
	BUILD_BUG_ON_MSG(!__same_type(*(ptr), ((type *)0)->member) &&	\
			 !__same_type(*(ptr), void),			\
			 "pointer type mismatch in container_of()");	\
	((type *)(__mptr - offsetof(type, member))); })
```
