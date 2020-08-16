

# Bare-metal Binary
운영체제 커널을 만드는 첫번째 스텝은 운영체제 없이 동작 할 수 있는 실행파일을 만드는 것이다.

운영체제를 만드는 것은 운영체제의 기능을 사용하지 않고 코드를 작성하는 것이다. 스레드, 파일 기능을 포함하여 러스트 표준 라이브러리(std crate) 또한 사용할 수 없다. 하지만 반복자, 클로저, 패턴 매칭, **소유권 시스템**과 같은 주요 러스트 기능은 사용할 수 있다. (core crate). 이 기능들로 C언어로 커널을 작성하는 것보다 높은 수준(high-level)으로 표현 할 수 있고, 매모리-세이프한 코드를 작성 할 수 있다. (이것이 러스트로 커널을 작성하는 주요 이유다.)

**운영체제없이 동작하는 실행파일을 "프리스탠딩(freestanding)" 혹은 "베어-메탈(bare-metal) 실행파일이라 한다.**


# Disabling the Standard Library
기본적으로 러스트로 작성하는 프로그램은 표준 라이브러리를 링크한다. *no_std attribute*를 사용하면 표준 라이브러리를 링크시키지 않는다. 표준 라이브러리를 링크에서 제외하면 표준 라이브러리에서 제공하는 실행파일을 만들기 위한 기능을 작성해야한다.

## Panic Implementation
*panic_handler attribute*는 프로그램에서 panic이 발생하면 실행시킬 함수를 지정하는 속성이다. 

## Disabling Unwinding
Panic이 발생하면 stack unwinding이 진행된다. stack unwinding은 OS 의존적인 라이브러리 libunwind를 사용하고, symbol information를 포함하기때문에 binary 크기가 증가한다.

Panic 발생 시 unwinding을 사용하지 않고 abort 시킨다.

## The start attribute
자바의 가비지 컬렉션, Go 언어의 소프트웨어 스레드와 같이 프로그래밍 언어는 자신의 고유의 기능을 가지고 있다. 런타임 시스템은 이러한 언어의 기능의 동작을 담당하는데*main()* 함수가 호출 되기 전 필요한 작업을 수행하기도 한다.

러스트는 C 런타임 라이브러리인 crt0 ("C runtime zero")를 사용하여 스택을 생성하고 인자들을 적절한 레지스터에 할당한다. 이 후 C 런타임은 러스트 런타임을 호출하는데, 런타임의 시작 포인트를 *start language item*으로 명시한다. 러스트 런타임은 스택 오버플로우 가드와 panic 발생 시 backrace를 표시하는 기능을 가지고 있으며 최소한의 기능을 포함하여 매우 작은(사이즈, 오버헤드) 런타임 시스템를 표방한다.

베어-메탈 실행파일은 러스트 런타임과 crt0의 기능을 사용할 수 없으므로 **고유의 엔트리 포인트를 정의**해야 한다. 기존의 엔트리 포인트를 덮어쓰는 방식으로 작성한다.

## Overwriting the Entry Point
러스트의 기본 엔트리 포인트를 사용하지 않는 속성은 *no_main*이다. 엔트리 포인트를 덮어쓰기 위해서 *_start()*함수를 작성한다.

엔트리 포인트는 추후 링커가 사용하기 때문에 컴파일러에 의해 name mangling이 이루어지지 않도록 *no_mangle*을 사용하고 C calling convention을 사용하기 위해 *extern "C"*를 사용한다.

## Linker
링커는 컴파일된 코드와 라이브러리를 결합시켜 실행파일로 만든다. 기본적으로 C 런타임을 사용하는 것으로 설정 되어있기 때문에 베어 베탈 타겟으로 빌드하는 방법이 필요하다.

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

