---
layout: post
title: Asynchronous programming on Rust
tags: [rust]
---

In order to
fully understand
`Asynchronous Prgramming`,
it's better to know below keywords.
- Sync, Async
- Blocking, Non-blocking IO
- Preemptive, Non-preemptive
- Concurrency, Parallelism

## Thread vs Async
#### Thread
Thread works preemptive way.
```
| thread A | | thread B |
| -------- | | -------- |
| stmt;    | |          |
|          | | stmt;    |
| stmt;    | |          |
|          | | stmt;    |
```

#### Aync (rust)
Asyn works non-preemptive way.
If async block wants to give up
control and allow other async blocks to run,
there are two ways.
`yield` and `await`.
```
| async A     | | async B     |
| -------     | | -------     |
| stmt;       | |             |
| stmt.await; | |             |
|             | | stmt;       |
|             | | stmt.await; |
| stmt;       | |             |
| stmt.await; | |             |
```
## Future
The Future trait is core on async/await.
*Actually rust compiler translates `async/await`
to `Future` and `Combinator`.*

Future represents
a value that
might not be available yet.
To get the value in future,
future should be run on executor.
Since rust does not provide default runtime,
we should use non-standard crates
like [tokio](https://github.com/tokio-rs/tokio),
[futures](https://docs.rs/futures/0.3.10/futures/).


For primer this example is nice.
```rust
// https://rust-lang.github.io/async-book/01_getting_started/04_async_await_primer.html
use futures::executor::block_on;

async fn hello_world() {
    println!("hello, world!");
}

fn main() {
    let future = hello_world(); // Nothing is printed
    block_on(future); // `future` is run and "hello, world!" is printed
}
```

The example runs `hello_world` asynchronous way.
Since `fn hello_world` has `async` keyword,
rust compiler translate it `Future`.

#### The Async/Await Pattern
Async/await let the programmer
write code that looks like
normal synchronous code.
`async` turns a synchronous function
into an asynchronous function
by compiler.
```rust
async fn foo() -> u32 {
    0
}

// the above is roughly translated by the compiler to:
fn foo() -> impl Future<Output = u32> {
    future::ready(0)
}
```
`await` used to retrieve
the asynchronous value of a future
without needing any closures or Either type.
```rust
// async / await
async fn example(min_len: usize) -> String {
    let content = async_read_file("foo.txt").await;
    if content.len() < min_len {
        content + &async_read_file("bar.txt").await
    } else {
        content
    }
}

// Future Combinator
fn example(min_len: usize) -> impl Future<Output = String> {
    async_read_file("foo.txt").then(move |content| {
        if content.len() < min_len {
            Either::Left(async_read_file("bar.txt").map(|s| content + &s))
        } else {
            Either::Right(future::ready(content))
        }
    })
}
```

#### Under the hood
```rust
pub trait Future {
    type Output;
    fn poll(self: Pin<&mut Self>, cx: &mut Context) -> Poll<Self::Output>;
}

// return value; poll-Poll
pub enum Poll<T> {
    Ready(T),
    Pending,
}
```

The `Output` specifies the type of the asynchronous value.
The `Poll enum` represents the future's status
that its value is ready or not. 
The `poll()` checks
if the value is available.
If future is not ready its value,
it returns `Poll::Pending` and
give up its control.
So, other futures can be execute
without blocking.

`Context` is important to improve performance.
It has `Waker` that can notify
to executor "I am ready to poll again!", "I should be polled!".
In other words it allows the asynchronous task
to signal that it is finished.

If Context does not exist,
executor runs busy-waiting way :(
*The executor have no way of knowing
 when a particular future could make progress.*

> Summary  
> - Executor calls future.poll()  
> - Future returns Poll::Pending or Poll::Ready(value)
> - If future is pending, **wake()** callback makes it more progress. (re-scheduling)


## Executor
Executor spawns futures as independent tasks.
Task represents top-level future.
Executor polls all futures until they are completed.
(switch to a different future whenever a future returns Poll::Pending)

There are several scheduling policies (join and block_on ...).

If you want to chain futures together,
call future's method.  
```rust
get_breakfast.and_then(|food| eat(food))
```

## Async with non-blocking
Since default tcp socket works blocking I/O,
there are no way to run on single-thread.
But we can make it with async +
[non-blocking](https://doc.rust-lang.org/std/net/struct.TcpStream.html#method.set_nonblocking) I/O :)

```
1. Make tcp server with non-blocking way
  1-1. async_accept()
  1-2. async_read()
  1-3. async_send()
2. Make tcp client with non-blocking way
  2-1. async_read()
  2-2. async_send()
3. Run tcp server and accept the client with non-blocking
  3-1. If not accepted, yield. (returns Poll::Pending)
  3-2. If accpeted, ready. (returns Poll::Ready)
4. Connect client to server
5. (Client) Request to server
  5-1. Send data from to server
  5-2. Yield
  5-3. Receive data from server
6. (Server) Reply to client
  6-1. Receive data from client
  6-2. Excute server's routine
  6-3. Send data to client
  6-4. Yield
```

## Reference
- https://rust-lang.github.io/async-book
- https://os.phil-opp.com/async-await/
- https://jblog.andbit.net/2019/11/10/rust-async-execution/
