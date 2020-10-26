---
layout: post
title: The serde with no_std
tags: [rust]
---

This post is about sample code about serde_json_core.
Previous post about serde is [here](http://bitboom.github.io/rust-serde-basis).

## [Serde with no_std](https://serde.rs/)
In order to use Serde
in a no_std context
*default-features* needs to be disabled.
And *derive* feature
needs to be added
for Serialize, Deserialize trait.

```rust
# Cargo.toml
[dependencies]
serde = { version = "1.0", default-features = false, features = ["derive"] }
```

## [Serde_json no_std](https://japaric.github.io/serde-json-core/serde_json_core/)
Use *serde-json-core*
instead of *serde-json*
in a no_std context.

```
# Cargo.toml
[dependencies]
serde = { version = "1.0", default-features = false, features = ["derive"] }
serde-json-core = "0.1.0"
heapless = "0.5.5"
```

```rust
use heapless::consts::*;
use serde::{Serialize, Deserialize};
use serde_json_core;

#[derive(Serialize, Deserialize, Debug)]
struct Point {
    x: i32,
    y: i32,
}

fn main() {
    let point = Point { x: 1, y: 2 };

    // Convert the Point to a JSON string.
    // U30 is the size of String (It should be specified for heapless String
    let serialized = serde_json_core::to_string::<U30, Point>(&point).unwrap();
    // Prints serialized = {"x":1,"y":2}
    println!("serialized = {}", serialized);

    // Another way to write above code
    let serialized: heapless::String<U30> = serde_json_core::to_string(&point).unwrap();
    println!("serialized = {}", serialized);

    // Convert the JSON string back to a Point.
    let deserialized: Point = serde_json_core::from_str(&serialized).unwrap();

    // Prints deserialized = Point { x: 1, y: 2 }
    println!("deserialized = {:?}", deserialized);
}
```

## string type
Rust-core and serde-json-core do not support
[std::string::String](https://doc.rust-lang.org/std/string/struct.String.html) type.

For string, we should use [core::str](https://doc.rust-lang.org/core/str/index.html) like below.
```rust
let hello: &str = "Hello, world!";
let serialized: heapless::String<U30> = serde_json_core::to_string(&hello).unwrap();
let deserialized: &str = serde_json_core::from_str(&serialized).unwrap();
println!("{}", deserialized);
```

Check [supported types](https://docs.rs/serde-json-core/0.1.0/serde_json_core/).

## Summary
It is reasonable
to specify the size of the string
in order not to use the rust heap allocator.
But it makes the API inconvenience.
