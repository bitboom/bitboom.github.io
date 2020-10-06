## Async/Await
async/await는 Rust에서 제공하는 비동기 함수를 작성하는 도구다.
컴파일러는 async 블록으로 감싸진 코드 블록을 Future를 리턴하는 비동기 함수로 변경한다.
Future trait는 코드를 상태머신으로 표현할 수 있는 trait이다.
일반적인 동기화 함수 호출은 다른 스레드들을 블록시키며 자신의 코드를 실행하지만,
Future로 호출된 코드는 자신의 상태에 따라 다른 Future들이 실행 될 수 있도록 논-블로킹 방식으로 실행된다.

```rust
async fn f1{ ... }
async fn f2{ ... }
async fn f3{ ... }

// f1 -> f2 -> f3
async fn fmain {
  f1.await;
  f2.await;
  f3.await;
}

block_on(fmain);

// f1 -> f2 -> f3 -> f1 -> f2
join!(f1, f2, f3);
```

## Future
- 비동기 방식으로 실제 함수가 호출 될 때 값을 계산해서 리턴해줌
- async의 가장 핵심적인 자료구조
- 호출되지 않으면 값을 계산하지 않음
- 코드 실행(값 계산)이 완료되면 Poll::Ready(result)을 통해 값(result)을 리턴
- 코드 실행이 완료되지 않았으면 Poll::Pending을 리턴하여 다른 Future가 실행 될 수 있도록 제어권을 넘겨줌
- 제어권을 넘겨주기전에 waker를 통해 값이 계산 되면 제어권을 가져올 수 있도록 콜백함수를 등록할 수 있음

Pin -- immovable

## Task

Task - Waker

Future는 한 번에 완료되지 않고 polled 되는게 일반적이다.
Polled를  되면 다른 Future에게 제어권을 넘겨준 것,
진행이 더 안되니, 다시 polling 되어야함
1. 스케쥴러가 정책에따라 future들을 polling함
2. Waker가 wake()를 호출하여 관련있는 task가 polling 되어야한다고 말해줌

Future가 polled 되는 것은 task의 일부임.
Task는 Executor에 넘겨지는 최상위 Future

- Waker provides a wake() method that can be used to tell the executor that the associated task should be awoken.

- wake: Task가 Executor에게 나 깨어나야한다고 말하는 것 

