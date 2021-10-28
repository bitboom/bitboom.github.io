---
layout: post
title: Cross compilation on rust
tags: [rust]
---

> To [cross-compile](https://www.gnu.org/software/automake/manual/html_node/Cross_002dCompilation.html) is to build on one platform a binary that will run on another platform.

## Host default target
When we type out `cargo build`,
the compiler builds with host default target.  
To check target manually, use `cfg`.
The target triple is `<arch>-<vendor>-<os>-<env>`.
```sh
$ rustc --print cfg
debug_assertions
panic="unwind"
target_arch="x86_64"
target_endian="little"
target_env="gnu"
...
target_os="linux"
target_pointer_width="64"
target_thread_local
target_vendor="unknown"
unix
```

Also, we can check it using `default-target` crate.

```sh
$ cargo install default-target
$ default-target
x86_64-unknown-linux-gnu
```

## [Cross compilation](https://rust-lang.github.io/rustup/cross-compilation.html#cross-compilation)
To cross compile, install target what we want to run on another platform.
```sh
rustup target add thumbv8m.main-none-eabi
```

Then, we can build like
```sh
cargo build --target thumbv8m.main-none-eabi
```

To set default target, write `.cargo/config` at crate root.
```sh
[build]
target = "thumbv8m.main-none-eabi"
```
