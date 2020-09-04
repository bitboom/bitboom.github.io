# [std::async](https://en.cppreference.com/w/cpp/thread/async)
- sync vs async => std::invoke vs std::async
- Concurrency != Parallel
- Async != Thread; but run on thread
- std::async is blocking
- std::async returns std::future
- How can we evaluate std::future?
	- [std::future::wait](https://en.cppreference.com/w/cpp/thread/future/wait)
	- **Blocks until the result becomes available**
- policy
	- std::launch::async; enable asynchronous evaluation
		- dtor calls wait()
	- std::launch::deferred; enable lazy evaluation
- std::future.wait() --- std::promise.set_value()


```cpp
std::async(std::launch::async, []{ std::cout << "1"; });
auto wait_by_dtor = std::async(std::launch::async, []{ std::cout << "3"; });
auto wait = std::async(std::launch::deferred, []{ std::cout << "2"; });
wait.wait();

// result: 123
```

- After all, what you want to do with asynchronous execution
  is to get some data processed through another thread (context).
- async execution = context

# System
- How system spawn actor?
- What run() means?
	- future를 input으로 받아서 block_on 시키는 것뿐. 그 이상도 아님
	- return void type Future
	- stop() 전까지 동작해야함. 꺼지지 말아야함..!
	- 그 future가 actor고 actor_ref를 통해 언제든 message를 받을 수 있는 상태라는 것
	- actor가 run안에 들어가면 살아 있으니까, mailbox에 들어오면 actor를 call함
	- 그렇다면 run은 사실상, actor가 busy-waiting되는 상태가 아닌 actor의 instance를 유지시켜주면 됨
	- 그럼 run 이후 actor의 instance가 있으면 계속 loop돌면 됨
	- stop하면 instance 초기화 시키고 종료하면됨
	- run은 하나의 future를 받음...
	- 다른 thread에 run한다고 생각하면 될 듯.
	- run 하고 나서 목적은 actor_ref를 통해 message를 받을 수 있게 하는 거
	- 그럼 actor_ref는 스레드간 통신할 수 있는 매게체

- System::run(actor) -> return actor_ref -> actor_ref.send(message1)
- -> mailbox(messages) -> dispatch(mailbox) -> actor.handle(message1)

- mailbox: <boost/lockfree/queue.hpp>

- Context간 통신은 어떻게 ?
	- Context에 있는 mailbox에 message를 어떻게 전달?
	- Channel 필요.
	- MPSC를 통해 넣나?
	- Context = 다른 실행흐름들

```
actor
  |- context
       |- mailbox

- singleton

- Future combinator가 메인? 

# Actor
- Callable object -> public: operate()
	- **template method pattern**  -> private: task()
	- operate() { self.task() }
- Context;
	- Actor execution context. Manages mailbox, actor Items.
	- System에서 run되고 나면 메일박스에 들어오는 것을 기다렸다가 들어오면 실행 시킴.
	- Async block
	- mailbox를 갖고 있음

# ActorRef
- Actor's address
- identifier
- communication channel between System(run)

```
Context                                Context
-------                                -------
mailbox (lock-free)                    mailbox
| <- ref (future.wait) -> channel ---> promise.set 
| <- ref (future.wait) -> channel ---> promise.set 
| <- ref (future.wait) -> channel ---> promise.set 
+-<- ref (future.wait) -> channel ---> promise.set 
```

```

// m1
Actor.start() ---------> ActorRef <----- ActorRef.send(Message)
	|										|
	|- context.run()						|
	|	|- Async
	|	|- MessageQueue						|
	|	|- Wait/Notify						|
	|- ActorRef.tx 							|- ActorRef.rx
	|	|- Receiver --------channel()		|- Sender
	|	|- Promise.set()					|- Future.get()
	|- Handler(MessageFuture.get())
