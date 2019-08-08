---
layout: post
title: Hello, Rust!
tags: [rust]
---

# [Rust](https://www.rust-lang.org/)
> A language empowering everyone to build reliable and efficient software.

Rust is powerful programming language in terms of Performance, Reliality, Productivity. 

In other words, Rust is a memory-safe language. One of the main reasons is to prohibit the misuse of pointer(memory) at compile time. (See [here](https://doc.rust-lang.org/nomicon/meet-safe-and-unsafe.html#meet-safe-and-unsafe) for more details.)  

Anyway, Let's start Rust.

## Install Rust (feat. Ubuntu 16.04)
```sh
$ curl https://sh.rustup.rs -sSf | sh
$ rustup update && rustup --version
...
rustup 1.18.3 (435397f48 2019-05-22)
```

## Register environment variable
```sh
$ cat ~/.cargo/env >> ~/.bashrc
$ cat ~/.bashrc | tail -1
export PATH="$HOME/.cargo/bin:$PATH"
$ source ~/.bashrc
```

## Say "Hello, Rust!"
Write down "hello.rs" and complie with rustc.
```rust
// hello.rs
fn main() { println!("Hello, Rust!"); }
```
```sh
$ rustc hello.rs && ./hello
Hello, rust!
```
I don't like curly bracket, but it's simple.  

## Use the formatter
Rust provides and automatic formatter called rustfmt. Let's try to use rustfmt.
```sh
$ rustfmt hello.rs
```
```rust
// hello.rs
fn main() {
    println!("Hello, Rust!");
}
```
It's convenience tool for fomatting.

## Thread programming
Refer the example [here](https://doc.rust-lang.org/rust-by-example/std_misc/threads.html).

The example makes 10 threads and writes numbers.
```rust
// thread.rs
use std::thread;

static NTHREADS: i32 = 10;

// This is the `main` thread
fn main() {
    // Make a vector to hold the children which are spawned.
    let mut children = vec![];

    for i in 0..NTHREADS {
        // Spin up another thread
        children.push(thread::spawn(move || {
            println!("this is thread number {}", i);
        }));
    }

    for child in children {
        // Wait for the thread to finish. Returns a result.
        let _ = child.join();
    }
}
```
```sh
$ rustc thread.rs && ./thread
this is thread number 0
this is thread number 7
this is thread number 4
this is thread number 8
this is thread number 5
this is thread number 3
this is thread number 1
this is thread number 2
this is thread number 6
this is thread number 9
```

I do not know well yet.. It does not look like concise.
I like C++ style so far.

```cpp
// thread.cpp
#include <thread>
#include <iostream>
#include <vector>

static int nthreads = 10;

int main() {
    std::vector<std::thread> children;

    for (int i = 0; i < nthreads; i++)
        children.emplace_back([] {
            std::cout << "this is thread number" << i << std::endl;
        });

    for (auto& child : children)
        child.join();

    return 0;
}
```
