---
layout: post
title: Calling Rust functions from C++
tags: [rust]
---

Let's talk about FFI(foreign function interface) on rust.
FFI provides the way of creating a C-friendly API.

```
+----------------------------+
| hello() -> | +-----------+ |
| main()     | | hello() {}| |
|            | +-----------+ |
------------------------------
| c++        | rust (lib)    |
+----------------------------+
```

## Write rust library
This library is called by c++.
Since rust uses `.rlib` extention for static library,
we have to make library **dynamic linking** called shared library.

```sh
# Cargo.toml
[lib]
crate-type = ["dylib"]
```

#### Hello world
Let's expose very simple function.
Compiler do `name mangling`
to reslove unique name of symbol.
The way `name mangling` is different
for each language (c++, rust).

So, we have to make compiler
to follow C's `name mangling`
with `no_mangling` attribute.
And we have to sync ABI
with `extern "c"`.

Like this,
```rust
// src/lib.rs
#[no_mangle]
pub extern fn hello() {
    println!("Hello world!");
}

// Same with above
#[no_mangle]
pub extern "c" fn hello() {
    println!("Hello world!");
}
```

Since rust uses
C ABI for extern keyword by default,
we can omit `"C"`.

#### Build
```sh
$ cargo build
# library path
$ ls target/debug/
libhello.so
```
## Call from c++

Declare that `hello function` is outside
with CABI `extern "C"`.

> The extern keyword defines a function 
> with a foreign calling convention and
> is often used to communicate with C code. (extern "C" fn)

```cpp
// hello.cpp
extern "C" void hello();

int main() {
    hello();
    return 0;
}
```

To build with rust library,
we have to let compiler know two things.

- Where is the library at compile time => -L option
- Whers is the library at execution time => -rpath option

Like this,
```sh
$ g++ hello.cpp -lhello -L ./target/debug/ -Wl,-rpath,./target/debug/
$ ./a.out
Hello world!
```

Finally, we can call rust library from c++!

## Primitive types
Now, we will pass primitive parameter types. `int, float, bool`

```rust
// src/lib.rs
#[no_mangle]
pub extern fn print(a: i32, b: f32, c: bool) {
    println!("int: {}, float: {}, bool: {}", a, b, c);
}
```

It's better to use fixed size type like int32_t.
But it's ok for example.

```cpp
// hello.cpp
extern "C" void print(int a, float b, bool c);

int main() {
    print(100, 100.3, false);
    return 0;
}
```

The way of execution is same with hello world.

## Advanced example with String
The advanced example is string type.
To treat string, we should know more things.
- [std::ffi::CStr](https://doc.rust-lang.org/std/ffi/struct.CStr.html)
- unsafe

To take `const char*` from C to rust,
we can use `CStr::from_ptr()`.
Since it is unsafe function,
`unsafe` keyword is needed.

```rust
// src/lib.rs
use std::ffi::CStr;
use std::os::raw::c_char;

#[no_mangle]
pub extern fn print_str(s: *const c_char) {
    let raw = CStr::from_ptr(s);

    if let Ok(msg) = raw.to_str() {
        println!("print str: {}", msg);
    }
}
```

Calling part is simpler than library side.
```cpp
extern "C" void print_str(const char *s);

int main() {
    print_str("genius..");
    return 0;
}
```

## Advanced example with Sturct
The last example is struct type.
For struct, we make sync between C's struct and Rust's struct.

[repr(c)](https://doc.rust-lang.org/nomicon/other-reprs.html) attribute
keep align rust's struct with c's struct.
We should define struct both c++ and rust.

```rust
// src/lib.rs
#[repr(C)]
pub struct Coordinate {
    x: i32,
    y: i32,
}

#[no_mangle]
pub extern fn flip(c: Coordinate) -> Coordinate {
    Coordinate {
        x: c.y,
        y: c.x,
    }
}
```

Check `struct Coordinate`.
```cpp
#include <iostream>

struct Coordinate {
	int x;
	int y;
};

extern "C" Coordinate flip(Coordinate);

int main(void) {
	auto flipped = flip(Coordinate { .x = 3, .y = 5 });
	std::cout << flipped.x << ", " << flipped.y << std::endl;
}
```

## Appendix
- [rust-ffi-omnibus](http://jakegoulding.com/rust-ffi-omnibus/)
