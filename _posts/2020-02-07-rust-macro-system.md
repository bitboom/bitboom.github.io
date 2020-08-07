---
layout: post
title: Rust Macro System
tags: [rust]
---

In this post,
uses declarative macros
to make DSL and variadic.

## Rust Macros
> Fundamentally,
> macros are a way of writing code
> that writes other code, which is known as metaprogramming.

Rust has powerful macro system
what is almost compiler plugin level.
The reason this is possible is that
rust allows you to handle Abstract Syntax Tree,
not just strings at preprocessor like C/C++.

There are 5-types macros.
- Declarative macros
- Procedural Macros
	- Custom derive Macro 
	- Attribute-like macros
	- Function-like macros

We can use 3-techniques with them.
- [DRY (Don't Repeat Yourself)](https://doc.rust-lang.org/rust-by-example/macros/dry.html)
- [Domain Specific Languages (DSLs)](https://doc.rust-lang.org/rust-by-example/macros/dsl.html)
- [Variadic Interfaces](https://doc.rust-lang.org/rust-by-example/macros/variadics.html)  
	- Rust does not provide variadic template like c++

## Declarative macros
#### Domain Specific Languages
Macros runs a way of **pattern matching**
on source code.
The patterns are compared with
the structure of that source code and
the code associated with each pattern,
when matched,
replaces the code passed to the macro.
**This all happens during compilation.**

Let's see the example which contains DSL technique.
```rust
macro_rules! calculate {
    (sum $e1:expr, $e2:expr) => {{
        {
            let var1: i32 = $e1;
            let var2: i32 = $e2;
            println!("The sum is = {}", var1 + var2);
        }
    }};
    (mul $e1:expr, $e2:expr) => {{
        {
            let var1: i32 = $e1;
            let var2: i32 = $e2;
            println!("The mul is = {}", var1 * var2);
        }
    }};

}

fn main() {
    calculate! {
        sum 1, 2
    }

    calculate! {
        mul 3, 4
    }
}
```
```sh
The sum is = 3
The mul is = 12
```

There are two brances at calculate macro.
One is *sum* and another is *mul*.

Within () is *$name:expr* which matches
any Rust expression and
gives the expression the name $name.
The each branch takes two expression *$e1 and $e2*.

We can force types to expression like *let var1: usize = $e1*.

This looks nice technique.

#### + Variadic
Let's take variadic arguments.
I will only apply it to *sum*.

```rust
macro_rules! calculate {
    (sum $($e:expr),*) => {{
        {
            let mut sum: i32 = 0;
            $(
                let elem: i32 = $e;
                sum += elem;
            )*
            println!("The sum is = {}", sum);
        }
    }};
    (mul $e1:expr, $e2:expr) => {{
        {
            let var1: i32 = $e1; // Force types to be integers
            let var2: i32 = $e2;
            println!("The mul is = {}", var1 * var2);
        }
    }};

}

fn main() {
    calculate! {
        sum 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    }

    calculate! {
        mul 3, 4
    }
}
```
```sh
The sum is = 55
The mul is = 12
```

The code looks a little weird but the code frame is simple.
It runs like regular expression.
```
1: $($e:expr),*

2: $(
3:     ...
4: )*
```
Line 1 takes variadic arguments by * keyword (0..n).
And make loop in line 2~4 for n times.

So, the code is unfolded like below.

```rust
/// 0..
let elem: i32 = 1;
sum += elem;

let elem: i32 = 2;
sum += elem;

...

/// ..n
let elem: i32 = 10;
sum += elem;
```

## Summary
We implemented DSL and variadic using declarative macros.
Macros looks like a little weird,
but it is very usable. 
If we use procedural macros together,
we can make more powerful code. (compiler-plugin level!!)
In the next post, let's look at procedural macros.

## References
- https://doc.rust-lang.org/book/ch19-06-macros.html
- https://doc.rust-lang.org/rust-by-example/macros.html
