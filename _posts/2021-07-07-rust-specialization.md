---
layout: post
title: Rust specialization with cpp specialization
tags: [rust]
---
 
Before introducing `specialization on rust`,
Let's remind `template specialization on cpp`.

## [Template specialization on cpp](https://en.cppreference.com/w/cpp/language/template_specialization)
### Scalar type
How can we get scalar type's size?
Scalar type's size is fixed at compile time,
it is always same at runtime.

We can get `size` via [size_of](https://en.cppreference.com/w/cpp/language/sizeof).

```cpp
sizeof(int)
sizeof(double)
...
```

### Compound type (w/ length type)
We cannot get size of `string` nor `vector` at compile time.
So, we have to use `string::length()` or `vector::size()`.

```cpp
std::string("foo").length()
std::vector<int>{1, 2, 3}.size()
```

### Make them generic
Let's make template function for getting size of variable.
I will use template specialization.
1. (Default) Use sizeof
2. (Special type) Use member function

```cpp
#include <iostream>
#include <string>
#include <vector>

template<typename T>
std::size_t size(T&& value)
{
	return sizeof(T);
}

template<>
std::size_t size(std::string&& string)
{
	return string.length();
}

template<typename T>
std::size_t size(std::vector<T>&& vector)
{
	return vector.size();
}
```

Let's run!
```cpp
int main() {
	// int - 4
	std::cout << size(0) << std::endl;

	// double - 8
	std::cout << size(0.0) << std::endl;

	// std::string - 4
	std::cout << size(std::string("text")) << std::endl;

	// std::vector - 3
	std::cout << size(std::vector<int>{1, 2, 3}) << std::endl;

	return 0;
}
```

## [Specialization on rust](https://en.cppreference.com/w/cpp/language/template_specialization)
Rust's specialization is unstable,
we should use nightly version.

The step is same with cpp.
1. (Default) Use sizeof
	- Rust use `default` keyword for it.
2. (Special type) Use member function


```rust
// For unstable feature
#![feature(min_specialization)]

use std::mem::size_of;

trait Measurable {
    fn size(&self) -> usize;
}

// Check default keyword
impl<T> Measurable for T {
    default fn size(&self) -> usize {
        // This is for scalar type
        size_of::<Self>()
    }
}

// Specialize vector type
impl<T> Measurable for Vec<T> {
    fn size(&self) -> usize {
        self.len()
    }
}

// Specialize string type
impl Measurable for String {
    fn size(&self) -> usize {
        self.len()
    }
}
```

Let's run!

```rust
fn main() {
    assert_eq!(Measurable::size(&true), 1);

    assert_eq!(Measurable::size(&0u8), 1);
    assert_eq!(Measurable::size(&0u16), 2);
    assert_eq!(Measurable::size(&0u32), 4);
    assert_eq!(Measurable::size(&0u64), 8);

    assert_eq!(Measurable::size(&String::from("foo")), 3);

    assert_eq!(Measurable::size(&vec![1, 2, 3, 4]), 4);
}
```
