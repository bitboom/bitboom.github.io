---
layout: post
title: Error handling(std::optional) from C++17 
tags: [c++]
---

에러 처리를 위해 C++17에 도입된 std::optional에 대하여 알아보자.
![c++17](https://www.modernescpp.com/images/blog/Functional/FunctionalInCpp17andCpp20/timeline.FunktionalInCpp17Cpp20Eng.png)


# [posix errno and c]([http://man7.org/linux/man-pages/man3/errno.3.html](http://man7.org/linux/man-pages/man3/errno.3.html))

시스템 프로그래밍에 사용하는 <errno.h>는 시스템 콜이나, 라이브러리 함수의 에러 처리를 위해 미리 정의된 정수형 값을 포함하고 있다. 대부분의 시스템 콜은 실패시 -1을 반환 하며, 라이브러리들은 에러를 표현하기 위하여 특정 값을 리턴하기도 한다.

```sh
$ errno -l
  EPERM 1 Operation not permitted
  ENOENT 2 No such file or directory
  ESRCH 3 No such process
  EINTR 4 Interrupted system call
  EIO 5 Input/output error
```

이와 같은 방식으로 대부분의 C 라이브러리들은 에러를 처리하기 위해 함수의 리턴 형을 사용한다.

# c++ (before c++17)
기본적인 c++의 에러 처리 방식은 exception based이다. exception으로 에러 처리를 한다면 함수를 정의 할 때 불필요한 요소를 제거 할 수 있다는 장점이 있다. (리턴 값에 에러 여부를 표현 하지 않아도 되기에)  

허나 exception 기반 에러 처리 방식은 함수를 호출 하는 입장에서 try_catch를 사용하여 추가적인 코드 작성을 해야하는 부담이 있고, try_catch의 위치에 따라 코드의 흐름을 정확히 파악하기 힘들다는 단점이 있다.

따라서 c++에서는 에러 처리를 위해 exception을 적재적소에 사용하여야 하고, 이의 보완책으로 c의 방식을 사용하거나, 아래의 두가지 방식으로 해결하기도 한다.

1. std::pair<bool, T>의 형식으로 bool값에 에러 여부를 확인한다.
2. std::unique_ptr<T>의 형식으로 에러일 경우 nullptr을 리턴한다.

1번의 경우 에러 발생 시, T의 기본 생성자를 리턴해야 해야하므로 T 타입의 불필요한 기본 생성자가 추가 될 수 있고, T의 타입에 따라 성능 오버헤드가 커질 수 있다. 2번의 경우 에러 발생 시 nullptr을 리턴하면 되므로 성능 오버헤드는 업지만 정상인 경우도 스마트 포인터 타입으로 리턴해야 하므로 가독성이 떨어진다.

# rust
Rust는 에러 처리 방식을 명료하게 두가지로 구분한다. Recoverable이라는 컨셉을 가져와서 에러의 종류를 unrecoverable error와 recoverable error로 구분하고, unrecoverable에 대해서는 panic!(abort)을 호출하여 프로그램을 종료시킨다.

Recoverable 에러에 대해서는 함수의 리턴형으로 Result라는 pre-defined된 enum을 사용한다. 정상 처리 되었을 경우 실제 T타입을 반환하고, 에러일 경우 E타입의 에러를 반환 한다.
```cpp
enum Result<T, E> {
  Ok(T),
  Err(E),
}
```

Result형을 반환하는 함수를 사용하는 쪽에서는 control flow operator인 match를 사용하거나, unwrap(), expect()와 같은 멤버 함수, 또는 shortcut을 위한 ? operator를 사용하여 다양한 방식으로 에러 처리를 할 수 있다.
```cpp
/// match operator
let f = File::open("hello.txt");
let f = match f {
  Ok(file) => file,
  Err(error) => { panic!("Problem opening the file: {:?}", error)
  },
};

/// additional methods
let f = File::open("hello.txt").unwrap();
let f = File::open("hello.txt").expect("Failed to open hello.txt");

/// shortcut ? operator
let  mut f = File::open("hello.txt")?;
```
 자세한 설명은 [document]([https://doc.rust-lang.org/book/ch09-02-recoverable-errors-with-result.html](https://doc.rust-lang.org/book/ch09-02-recoverable-errors-with-result.html))를 참고 바란다.
 
# since c++17
c++17에서는 에러 처리를 위하여 [std::optional](https://en.cppreference.com/w/cpp/utility/optional)이라는 class template이 도입 되었다. std::optional의 의도는 rust에서 표현한 recoverable 에러에 대한 처리 방식으로 보면 된다. rust와의 차이점은 template의 인자 갯수를 보면 알 수 있는데, error를 나타내는 template이 rust에 있는 반면, c++에는 에러 시에는 단순히 std::nullopt를 반환 한다.

```cpp
template<  class T >  
class optional;
```

실제 std::optional을 반환형으로 사용하는 함수를 구현해보면 아래와 같다. 성공시 실제 타입을 리턴하면 되고, 에러를 나타내야 할 때는 empty brace를 사용하면 된다.
```cpp
#include <optional>
 
// optional can be used as the return type of a factory that may fail
std::optional<std::string> foo() {
    if () // success
        return "success";
    else // fail
        return {};
}
```

이를 다른 방식으로 표현 할 수도 있는데, 코드로 나타내면 아래와 같다.
```cpp
auto foo() {
    return b ? std::optional<std::string>{"success"} : std::nullopt;
}
```

실제 함수를 사용하는 쪽에서는 * operator를 사용하거나 value() 함수를 사용하여 값을 구할 수 있다. 에러가 발생 했을 때는 nullptr과 같은 방식으로 확인하면 되고, 에러가 발생 했을 때 default 값을 설정 할 수 있는 [value_or()](https://en.cppreference.com/w/cpp/utility/optional/value_or) 함수도 제공한다.
```cpp
auto str = foo()

/// check error
if (str) { /// success
  std::cout << str.value() << std::endl;
  std::cout << *str << std::endl; // same with above linesjf
} else {
  /// fail
}

/// set default value if value is not exist\
std::cout << str.value_or("default value") << std::endl;
```

std::optional에 대한 추가적인 멤버 함수는 [reference]((https://en.cppreference.com/w/cpp/utility/optional))를 참조하기 바란다.


# Summary
c++17에서는 추가적인 에러(recoverable) 처리 방식으로를 std::optional을 도입하였다.
