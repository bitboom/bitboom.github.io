---
layout: post
title: Introduce unit & benchmark test on rust
tags: [rust]
---

## [Rust](https://www.rust-lang.org/)
- Modern system programming language
- Designed for performance and safety, especially safe concurrency
- Similar to Modern C++, but can guarantee memory safety
  - Using a borrow checker to validate references
- Achieves memory safety without garbage collection

---

## [Rust Adotion](https://en.wikipedia.org/wiki/Rust_(programming_language/))

- Web browser
  - Servo, browser engine
  - Quantum, next-generation web engine for Firefox
- Operating systems
  - Redox
  - Stratis
  - Google Fuchsia

---

## [Testing](https://doc.rust-lang.org/stable/rust-by-example/testing.html#testing) on Rust
Rust supports for writing software tests within the **language itself**. 
Testing comes in four styles:
- Unit testing.
- Doc testing.
- Integration testing.
- Benchmark testing. (unstable)

Let's see unit-testing and benchmark-testing.

---

### Unit test sequence
1. Write code we want to test
2. Check whether the results are what we expect

```rust
let result = add(1, 2);
assert_eq!(result, 3);
```
---

### How to write test functions
`test function` in `tests module`
1. Write `tests` module with `#[cfg(test)]` attribute
2. Write test functions with `#[test]` attribute.
	-  so the test runner knows to treat this function as a test

### Practice in [online ide](https://play.rust-lang.org/)
Run next page's code

---

```rust
#[cfg(test)]
mod tests {
	fn add(a: i32, b: i32) -> i32
	{
		a + b
	}

	fn bad_add(a: i32, b: i32) -> i32
	{
		a - b
	}

    #[test]
    fn test_add() {
        assert_eq!(add(1, 2), 3);
    }

    #[test]
    fn test_bad_add() {
        assert_eq!(bad_add(1, 2), 3);
    }
}
```

---

### How to run test on cli
```sh
$ cargo test
running 2 tests
test tests::test_add ... ok
test tests::test_bad_add ... FAILED
```

---

### Testing panics
To check functions that,
should panic under certain circumstances,
use attribute `#[should_panic]`.

---

```rust
#[cfg(test)]
mod tests {

    fn divide(a: u32, b: u32) -> u32 {
        if b == 0 {
            panic!("Divide-by-zero error");
        }

        a / b
    }

    #[test]
    fn test_divide() {
        assert_eq!(divide(10, 2), 5);
    }

    #[test]
    #[should_panic]
    fn test_any_panic() {
        divide(1, 0);
    }
}
```

---

### Testing ignore
Test function can be marked
with the `#[ignore]` attribute 
to exclude some tests.

---

```rust
// cargo test
// cargo test -- --ignored (Run only ignored test)

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_add() {
        assert_eq!(add(2, 2), 4);
    }

    #[test]
    fn test_add_hundred() {
        assert_eq!(add(100, 2), 102);
        assert_eq!(add(2, 100), 102);
    }

    #[test]
    #[ignore]
    fn ignored_test() {
        assert_eq!(add(0, 0), 0);
    }
}
```

---

### [Benchmark tests (nightly, unstable)](https://doc.rust-lang.org/unstable-book/library-features/test.html)
Benchmark tests, which can test the performance of your code
- Unlike regular tests, which take no arguments, benchmark tests take a `&mut Bencher`
- This Bencher provides an iter method, which takes a closure
	- **This closure contains the code we'd like to benchmark.**
	- non-benchmark test was ignored
- Rust runs **our benchmark a number of times, and then takes the average**
	- with `iter loop`

---

### Tip for benchmark
#### Accuracy
- Move setup code outside the iter loop
	- only put the part you want to measure inside
- Make the code in the iter loop do something simple
	- to assist in pinpointing performance improvements

---

#### Consistency
- Make the code do "the same thing" on each iteration;
	- do not accumulate or change state
- Make the outer function idempotent too;
	- the benchmark runner is likely to run it many times

#### Performance
- Make the inner iter loop short and fast
	- so benchmark runs are fast and
	- the calibrator can adjust the run-length at fine resolution

---

```rust
#![feature(test)]
extern crate test;

pub fn add_two(a: i32) -> i32 { a + 2 }

#[cfg(test)]
mod tests {
    use super::*;
    use test::Bencher;

    #[bench]
    fn bench_add_two(b: &mut Bencher) {
        b.iter(|| add_two(2));
    }
    #[bench]
    fn bench_xor_1000_ints(b: &mut Bencher) {
        b.iter(|| {
            (0..1000).fold(0, |old, new| old ^ new);
        });
    }
    #[bench]
    fn bench_xor_1000_ints_no_optimizer(b: &mut Bencher) {
        b.iter(|| (0..1000).fold(0, |old, new| old ^ new));
    }
}
```

---

```sh
$ cargo bench
    Finished bench [optimized] target(s) in 0.00s
     Running unittests (target/release/deps/bench-f7df3e0ebfa630fc)

running 4 tests
test tests::it_works ... ignored
test tests::bench_add_two                    ... bench:           0 ns/iter (+/- 0)
test tests::bench_xor_1000_ints              ... bench:           0 ns/iter (+/- 0)
test tests::bench_xor_1000_ints_no_optimizer ... bench:          42 ns/iter (+/- 3)
```
