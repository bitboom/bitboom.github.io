

## 베어-메탈 바이너리
- 운영체제 커널을 만드는 첫번째 스텝은 운영체제 없이 동작 할 수 있는 실행 파일을 만드는 것 => 베어-메탈 바이너리
- 베어-메탈 바이너리는 하드웨어 위에 바로 동작 할 수 있는 바이너리
- std 표준 라이브러리, 운영체제 라이브러리를 포함한 rich 라이브러리를 사용할 수 없음
- **운영체제없이 동작하는 실행파일을 "프리스탠딩(freestanding)" 혹은 "베어-메탈(bare-metal) 실행파일

## 표준 라이브러리 비활성화
- 기본적으로 러스트로 작성하는 프로그램은 표준 라이브러리를 링크
- `no_std attribute`를 사용하여 표준 라이브러리를 링크시키지 않음
- 표준 라이브러리를 링크에서 제외하면 표준 라이브러리에서 제공하는 실행파일을 만들기 위한 기능을 작성함

### 패닉 핸들러 정의
- `panic_handler attribute`는 프로그램에서 panic이 발생하면 컴파일러가 호출하는 함수를 지정
- `struct PanicInfo`는 패닉이 발생한 위치를 담고 있음

### Language Item; 컴파일러 내부에서 사용하는 특별한 함수와 타입
- `trait Copy`는 컴파일러가 해당 타입이 copy semantic을 따르는 타입 => `#[lang = "copy"]`
- `#[lang = "eh_personality"]`는 stack unwinding을 구현한 함수를 표시
- `start`는 entry point를 지정
- 스택 언와인딩은 패닉이 발생했을 때 스택에 있는 변수들의 소멸자를 불러 메모리를 정리
- Language Item을 직접 구현하는 것은 위험한 행동

### 언와인딩 비활성화
- os 의존적인 부분이 있음
- binary size 증가
- panic 발생 시 abort 하는 것으로 비활성화
- Panic이 발생하면 stack unwinding이 진행
- stack unwinding은 OS 의존적인 라이브러리 libunwind를 사용하고
- symbol information를 포함하기때문에 binary 크기가 증가한다.
- Panic 발생 시 unwinding을 사용하지 않고 abort 시킨다.

### main 함수 호출 전; **런타임**의 역할
자바의 가비지 컬렉션, Go 언어의 소프트웨어 스레드와 같이 프로그래밍 언어는 자신의 고유의 기능을 가지고 있다. 런타임 시스템은 이러한 언어의 기능의 동작을 담당하는데*main()* 함수가 호출 되기 전 필요한 작업을 수행하기도 한다.

러스트는 C 런타임 라이브러리인 crt0 ("C runtime zero")를 사용하여 스택을 생성하고 인자들을 적절한 레지스터에 할당한다. 이 후 C 런타임은 러스트 런타임을 호출하는데, 런타임의 시작 포인트를 *start language item*으로 명시한다. 러스트 런타임은 스택 오버플로우 가드와 panic 발생 시 backrace를 표시하는 기능을 가지고 있으며 최소한의 기능을 포함하여 매우 작은(사이즈, 오버헤드) 런타임 시스템를 표방한다.

베어-메탈 실행파일은 러스트 런타임과 crt0의 기능을 사용할 수 없으므로 **고유의 엔트리 포인트를 정의**해야 한다. 기존의 엔트리 포인트를 덮어쓰는 방식으로 작성한다.
- main 함수 호출 전, 본 프로그램이 실행 되기 전, 언어마다 런타임을 사용함
- 런타임은 가비지 콜렉션, 소프트웨어 스레드 (go-routine)을 담당, 초기화
- 러스트 표준 라이브러리는 c언어의 crt0("C runtime zero")를 호출함
- crt0는 스택을 생성하고, 레지스터를 placing
- crt0는 자기 작업이 끝나고 rust 런타임의 entry point를 호출함
	- 이게 `start lang item`
- 러스트는 최소한의 런타임을 가지고 있음
	- 스택-오버플로우 가드
	- 패닉시 백-트레이스 찍는 로직
- 러스트의 런타임은 최종적으로 main을 호출

### Overwriting the Entry Point
- `start lang item`을 구현하는 것은 결국 crt0를 필요로 함
- 대신에 **crt0의 엔트리 포인트를 덮어써버림** => `_start()`
- `#![no_main]`으로 컴파일러에게 디폴트 entry point를 사용하지 않는다고 표시

러스트의 기본 엔트리 포인트를 사용하지 않는 속성은 *no_main*이다. 엔트리 포인트를 덮어쓰기 위해서 *_start()*함수를 작성한다.
엔트리 포인트는 추후 링커가 사용하기 때문에 컴파일러에 의해 name mangling이 이루어지지 않도록 *no_mangle*을 사용하고 C calling convention을 사용하기 위해 *extern "C"*를 사용한다.

## 리턴하지 않는 함수; ! return type
- 왜 필요? 엔트리 포인트는 운영체제 또는 부트로더에 의해서만 호출 됨
- 다른 함수에서 불리지 않음
- 엔트리 포인트는 값을 리턴하는 대신 `exit` 시스템콜을 호출함

## Linker
- 링커는 컴파일된 코드와 라이브러리를 결합시켜 실행파일로 만듬
- 기본적으로 C 런타임을 사용하는 것으로 설정 되어있기 때문에 베어 베탈 타겟으로 빌드하는 방법이 필요
- 링커에게 c 런타임을 사용하지 않겟다고 argument 줘야함

## 링커 아규먼트
- _start에서 부르는 함수 들을 못찾아서 에러남, (libc)에 있음
- we can tell the linker that it should not link the C startup routine by passing the -nostartfiles flag
- One way to pass linker attributes via cargo is the cargo rustc command. The command behaves exactly like cargo build, but allows to pass options to rustc, the underlying Rust compiler. rustc has the -C link-arg flag, which passes an argument to the linker. Combined, our new build command looks like this:

cargo rustc -- -C link-arg=-nostartfiles


## Building for a Bare Metal Target
러스트는 빌드 타겟을 명시하기 위해 *target triple*을 사용한다.
타겟 트리플은 CPU arch (x86_64), 벤더 (unknown), 운영체제 (linux), ABI (gnu)로 구성된다.
```
x86_64-apple-darwin
x86_64-unknown-linux-gnu
thumbv7em-none-eabihf
```
우리는 운영체제가 없는 환경을 타겟을 사용하기 때문에 *thumbv7em-none-eabihf*를 사용할 것이다.

# Summary
```rust
// src.main.rs

#![no_std] // don't link the Rust standard library #![no_main] // disable all Rust-level entry points
use core::panic::PanicInfo;
#[no_mangle] // don't mangle the name of this function pub extern "C" fn _start() -> ! {
// this function is the entry point, since the linker looks for a function // named `_start` by default
loop {}
}
/// This function is called on panic.
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
loop {} }
```

```sh
# Cargo.toml

[package]
name = "crate_name"
version = "0.1.0"
authors = ["Author Name <author@example.com>"]
# the profile used for `cargo build`
[profile.dev]
panic = "abort" # disable stack unwinding on panic
# the profile used for `cargo build --release`
[profile.release]
panic = "abort" # disable stack unwinding on panic
```

```sh
cargo build --target thumbv7em-none-eabihf
```
