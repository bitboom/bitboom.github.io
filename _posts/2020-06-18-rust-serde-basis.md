---
layout: post
title: The basis of serde on Rust
tags: [rust]
---

# [Serde](https://serde.rs/)
Serde is a framework
**for serializing and deserializing Rust data structures**.
A data structure that knows
how to serialize and deserialize itself is
one that implements Serde's Serialize and Deserialize traits.
Or **uses Serde's derive attribute**
to automatically generate implementations at compile time.
**(Highly recommended)**

# Serde with JSON
Let's see Serde example with JSON format.
It's very simple.
We just added "derive" attribute
and call "to_string(), from_str()".

```
# Cargo.toml
[dependencies]
serde = { version = "1.0", features = ["derive"] }
serde_json = "1.0.55"
```

```rs
use serde::{Serialize, Deserialize};

#[derive(Serialize, Deserialize, Debug)]
struct Point {
    x: i32,
    y: i32,
}

fn main() {
    let point = Point { x: 1, y: 2 };

    // Convert the Point to a JSON string.
    let serialized = serde_json::to_string(&point).unwrap();

    // Prints serialized = {"x":1,"y":2}
    println!("serialized = {}", serialized);

    // Convert the JSON string back to a Point.
    let deserialized: Point = serde_json::from_str(&serialized).unwrap();

    // Prints deserialized = Point { x: 1, y: 2 }
    println!("deserialized = {:?}", deserialized);
}
```

# [Serde with Binary](https://github.com/servo/bincode/blob/master/examples/basic.rs)
We can simply convert between struct and binary using bincode.
*"serde_derive" provides convenient macro "serialize() and deserialize()".*

If we are able to convert data struct to binary,
we can use it while server-client communication. :smiley:

```
# Cargo.toml
[dependencies]
serde = { version = "1.0", features = ["derive"] }
serde_derive = "1.0.27"
bincode = "1.2.1"
```

```rs
#[macro_use]
extern crate serde_derive;
extern crate bincode;

use bincode::{deserialize, serialize};

#[derive(Serialize, Deserialize, PartialEq, Debug)]
struct Entity {
    x: f32,
    y: f32,
}

#[derive(Serialize, Deserialize, PartialEq, Debug)]
struct World(Vec<Entity>);

fn main() {
    let world = World(vec![Entity { x: 0.0, y: 4.0 }, Entity { x: 10.0, y: 20.5 }]);

    let encoded: Vec<u8> = serialize(&world).unwrap();

    // 8 bytes for the length of the vector (usize), 4 bytes per float.
    assert_eq!(encoded.len(), 8 + 4 * 4);

    let decoded: World = deserialize(&encoded[..]).unwrap();

    assert_eq!(world, decoded);

    println!("decoded = {:?}, {:?}", world, decoded);
}
```

# Summary
Serde makes rust code simple.
If we declare one attribute, we can convert data structure to various formats.
