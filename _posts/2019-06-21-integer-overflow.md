---
layout: post
title: Integer overflow
tags: [security]
---

> In computer programming, an integer overflow occurs when an arithmetic operation attempts to create a numeric value that is **outside of the range** that can be represented with a given number of digits.

For easy understanding, let's take 8-bit integer.

| type  | bit  | min  | max  |
|:---:|:---:|:---:|:---:|
| int8_t | 8 | -(2^7) = -128 | 2^7 - 1 |
| uint8_t | 8 | 0 | 2^8 - 1 = 255 |

---

# 8-bit unsigned int overflow
uint8_t can represent 0 ~ 255. According to wraparound, 255 + 1 is zero not 256.  
*This is guaranteed by the C standard.*

> Wraparound: wrapping of numerical value in case of underflow or overflow

```cpp
uint8_t ui = 255;
/// ++ui is 0 - overflow occured
```
---

# 8-bit signed int overflow
In case of int8_t, it can represent -128 ~ 127. According to wraparound, 127 + 1 is -128 not 128.  
	
```cpp
int8_t i = 127;
/// ++ui is -128 - overflow occured
```

But, The C Standard says that if a program has [**signed integer overflow its behavior is undefined.**](https://www.gnu.org/software/autoconf/manual/autoconf-2.63/html_node/Integer-Overflow-Basics.html)
**And the undefined behavior can even precede the overflow.**

---

# Why signed overflow is undefined behavior?
There are two ways to represent signed int. One is one's complemnet, another is two's complement.

| type | bit  | min  | max  | zero |
|:---:|:---:|:---:|:---:|:---:|
| one's | 8 | -(2^7) + 1 = -127 | 2^7 - 1 = 127 | -0, +0 |
| two's | 8 | -(2^7) = -128 | 2^7 - 1 = 127 | 0|

Operations of signed-int depends on ALU(Arithmetic logic unit). So, compiler can not expect the value of overflow. (127 + 1 is -128? or -127?)

But since most of latest hardwares use two's complemnt, we can exepect it. (Refer [this](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1407r0.pdf
).)

Anyway signed overflow is undefined behavior.

---

# Simple detection of overflow, underflow
## 8-bit unsigned value
- overflow: a + b > b, (255 + 1 = 0) > 1
- underflow: a - b < a, (0 - 1 = 255) < 0

## 8-bit signed value
- overflow: positive a + positive b == negative c, 127 + 1 == -128
- underflow: negative a + negative b == positive c, (-128) + (-1) == 127
- additional: negative a * -1 == negative a,
	- -128 * -1 == 128, but,  8-bit signed value cannot represent 128 => -128 

---

# Compiler built-in functions with overflow checking
We can detect overflow with several ways. But compiler can remove our overflow checking statement for optimazation.  
(In simple terms, a + b > b is always true.)

**So, the best way for preventing overflow is using built-in functions.** Below is [gcc's build-in functions](https://gcc.gnu.org/onlinedocs/gcc/Integer-Overflow-Builtins.html).

```
bool __builtin_add_overflow (type1 a, type2 b, type3 *res)
bool __builtin_sadd_overflow (int a, int b, int *res)
bool __builtin_saddl_overflow (long int a, long int b, long int *res)
bool __builtin_saddll_overflow (long long int a, long long int b, long long int *res)
bool __builtin_uadd_overflow (unsigned int a, unsigned int b, unsigned int *res)
bool __builtin_uaddl_overflow (unsigned long int a, unsigned long int b, unsigned long int *res)
bool __builtin_uaddll_overflow (unsigned long long int a, unsigned long long int b, unsigned long long int *res)
```
