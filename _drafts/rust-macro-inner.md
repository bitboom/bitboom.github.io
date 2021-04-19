## Syntax extention; 구문 확장
- macro_rules!: user-defined macro

## 컴파일러에 의해 러스트 소스가 어떻게 프로세싱 되나?

## macro_rules는 어떻게 빌트되나?

## 컴파일러
0. 소스 코드
1. 어휘 분석기 (Lexical analyzer)
	- 토큰화
2. 구문 분석기 (Syntax analyzer)
	- 구문 트리
3. 의미 분석기 (Semantic analyzer)
4. 코드 생성기 (Code generator)

## Tokenization; 토큰화
- 러스트 프로그램의 컴파일 첫번째 단계는 토큰화
- 소스코드를 의미있는 단어들의 연속으로 분리

- Identifiers: foo, Bambous, self, we_can_dance, LaCaravane, …
- Literals: 42, 72u32, 0_______0, 1.0e-40, "ferris was here", …
- Keywords: _, fn, self, match, yield, macro, …
- Symbols: [, :, ::, ?, ~, @1, …


## Parsing
- 토큰화 다음 단계는 파싱
- 토큰의 스트림을, 추상 구문 트리로 변환시키는 것
- AST; Abstract Syntax Tree


## 매크로 처리
- AST가 생성되고 나서의 단계

## 토큰트리

- 토큰 -> 토큰트리 -> AST
- 모든 토큰은 토큰-트리
  - 토큰-트리의 leaf node (자식이 없는 노드)
- leaf 노드가 아닌 유일한 기본 토큰은 grouping 토큰
  - (..), [..], {..}

```
a + b + (c + d[0]) + e
```

- 위 statement는 단일 루트 트리가 아니라
- 루트-레벨에 7개의 토큰 트리가 있음.

```
 1.  2.  3   4.    5.    6.  7. 
«a» «+» «b» «+» «(   )» «+» «e»
          ╭────────┴──────────╮
           «c» «+» «d» «[   ]»
                        ╭─┴─╮
                         «0»
```

```
              ┌─────────┐
              │ BinOp   │
              │ op: Add │
            ┌╴│ lhs: ◌  │
┌─────────┐ │ │ rhs: ◌  │╶┐ ┌─────────┐
│ Var     │╶┘ └─────────┘ └╴│ BinOp   │
│ name: a │                 │ op: Add │
└─────────┘               ┌╴│ lhs: ◌  │
              ┌─────────┐ │ │ rhs: ◌  │╶┐ ┌─────────┐
              │ Var     │╶┘ └─────────┘ └╴│ BinOp   │
              │ name: b │                 │ op: Add │
              └─────────┘               ┌╴│ lhs: ◌  │
                            ┌─────────┐ │ │ rhs: ◌  │╶┐ ┌─────────┐
                            │ BinOp   │╶┘ └─────────┘ └╴│ Var     │
                            │ op: Add │                 │ name: e │
                          ┌╴│ lhs: ◌  │                 └─────────┘
              ┌─────────┐ │ │ rhs: ◌  │╶┐ ┌─────────┐
              │ Var     │╶┘ └─────────┘ └╴│ Index   │
              │ name: c │               ┌╴│ arr: ◌  │
              └─────────┘   ┌─────────┐ │ │ ind: ◌  │╶┐ ┌─────────┐
                            │ Var     │╶┘ └─────────┘ └╴│ LitInt  │
                            │ name: d │                 │ val: 0  │
                            └─────────┘                 └─────────┘
```

- AST와 토큰-트리를 구분해야함

---

## Macros in the AST; 
- 매크로 프로세싱은 AST가 생성된 이후에 진행
- 따라서 매크로에 사용되는 신택스는, 맞아야함

러스트 구문 확장 (syntax extension)은 네가지로 이루어짐

- `# [ $arg ]; e.g. #[derive(Clone)], #[no_mangle], …`
- `# ! [ $arg ]; e.g. #![allow(dead_code)], #![crate_name="blang"], …`
- $name ! $arg; e.g. println!("Hi!"), concat!("a", "b"), …
- $name ! $arg0 $arg1; e.g. macro_rules! dummy { () => {}; }.

- 3번째는 매크로 콜, 인보크
- 4번째는 유저 매크로 정의; macro_rules!

- 이 두개 모두 고정! $arg / $arg1 $arg2
- 즉, 싱글 토큰트리여야함
- 논-리프 토큰 : 괄호들 올 수 있음. (..), [..], {..}

- 3번쨰
```rust
bitflags! {
    struct Color: u8 {
        const RED    = 0b0001,
        const GREEN  = 0b0010,
        const BLUE   = 0b0100,
        const BRIGHT = 0b1000,
    }
}

lazy_static! {
    static ref FIB_100: u32 = {
        fn fib(a: u32) -> u32 {
            match a {
                0 => 0,
                1 => 1,
                a => fib(a-1) + fib(a-2)
            }
        }

        fib(100)
    };
}

vec![RED, GREEN, BLUE];

println!("Hello, World!");


bitflags! ⬚

lazy_static! ⬚

vec! ⬚;
println! ⬚;
```

- 파서는 ⬚에대해 분석하지 않음
- The **input** to every macro is a **single non-leaf token tree**.

- Macros (really, syntax extensions in general) are parsed as part of the abstract syntax tree.

- 매크로는 AST의 일부,

그래서 지정된 위치에만 나타 날 수 있음.

- Patterns
- Statements
- Expressions
- Items(this includes impl Items)
- Types

매크로가 될 수 없는 곳.
Some things not on this list:
- Identifiers
- Match arms
- Struct fields

## 확장
- AST가 생성 후, 의미를 부여하기전 (SEMANTIC), 매크로를 확장함
- 매크로 확장은 AST를 순회하는 것을 포함
  - **매크로 호출을 찾아 확장으로 대체함**

- **컴파일러는 AST를 가져와 매크로 호출 노드를 -> 매크로 출력 노드로 바꿈**

- 매크로 확장의 결과 (출력 노드)
  - an expression,
   - a pattern,
  - a type,
  - zero or more items, or
  - zero or more statements

### 매크로 확장 전 AST
```
┌─────────────┐
│ Let         │
│ name: eight │   ┌─────────┐
│ init: ◌     │╶─╴│ BinOp   │
└─────────────┘   │ op: Mul │
                ┌╴│ lhs: ◌  │
     ┌────────┐ │ │ rhs: ◌  │╶┐ ┌────────────┐
     │ LitInt │╶┘ └─────────┘ └╴│ Macro      │
     │ val: 2 │                 │ name: four │
     └────────┘                 │ body: ()   │
                                └────────────┘
```
```rust
let eight = 2 * four!();
```

### 매크로 확장 후 AST
```
┌─────────────┐
│ Let         │
│ name: eight │   ┌─────────┐
│ init: ◌     │╶─╴│ BinOp   │
└─────────────┘   │ op: Mul │
                ┌╴│ lhs: ◌  │
     ┌────────┐ │ │ rhs: ◌  │╶┐ ┌─────────┐
     │ LitInt │╶┘ └─────────┘ └╴│ BinOp   │
     │ val: 2 │                 │ op: Add │
     └────────┘               ┌╴│ lhs: ◌  │
                   ┌────────┐ │ │ rhs: ◌  │╶┐ ┌────────┐
                   │ LitInt │╶┘ └─────────┘ └╴│ LitInt │
                   │ val: 1 │                 │ val: 3 │
                   └────────┘                 └────────┘
```

```rust
let eight = 2 * (1 + 3);
```

## MACRO_RULES!
- macro_rules는 그 자체로 구문 확장임
- 러스트 문법의 일부가 아님

- 괄호는 어느 것을 사용해도 무관함

```rust
macro_rules! $name {
    $rule0 ;
    $rule1 ;
    // …
    $ruleN ;
}
```

- 최소한 하나의 룰을 포함해야함
- 각 룰을 아래처럼 생김 -- 패턴 매칭
  - `($matcher) => {$expansion}`
  - 위 괄호들을 사용하는게 관행

- expansion은 룰의 transcriber라 불림
- 확장은 변환기라 불림

- 매크로_룰이 호출되면 인터프리터는 순서대로 룰을 하나씩 해석함
- 인풋이 matcher와 일치하면 invocation이 expansion으로 대치 됨.

```rust
macro_rules! macro {
  ($matcher) => {$expansion}
}

- let ret = macro!(input);
+ let ret = expansion(input);
```

## Matchers can also contain literal token trees, which must be matched exactly.

- matcher는 문자열 토큰-트리들을 포함 할 수 있음, 정확히 일치한다면.

## Metavariables
- matcher는 **capture**를 포함 할 수 있음

- 일반적인 문법 범주에 따라 입력을 일치
- 결과를 **메타 변수로 캡처 한 다음 출력으로 대체**

### 캡처 구성
- Captures are written as a dollar ($) followed by an identifier, a colon (:), and finally the kind of capture which is also called the fragment-specifier, which must be one of the following:

- $ + : + 캡처
  - ex) $e:expr // metavariable $e
  - $name:expr
  - $name으로 참조,
  - stmt: a statement
  - tt: a single token tree
  - ty: a type

- [캡처, 프레그먼트 스페시파이어](https://veykril.github.io/tlborm/macros/minutiae/fragment-specifiers.html)

- 메타 변수는 러스트 컴파일러의 파서를 이용함 -- 정확
- expr은 항상 컴파일 되는 표현식을 캡처함

### multiple metavariables in a single matcher
```rust
macro_rules! multiply_add {
    ($a:expr, $b:expr, $c:expr) => { $a * ($b + $c) };
}
```

```
macro_rules! discard {
    ($e:expr) => {};
}
macro_rules! repeat {
    ($e:expr) => { $e; $e; $e; };
}
```

## Repetitions
- **Matchers can contain repetitions**
- form: `$ ( ... ) sep rep`
  - sep is an optional separator token
  - It may not be a delimiter or one of the **rep**etition operators
  - Common examples are , and ;
  - rep is the required repeat operator
    - ?: indicating at most one repetition
    - *: indicating zero or more repetitions
    - +: indicating one or more repetitions

### Repetitions can contain any other valid matcher

```rust
macro_rules! vec_strs {
    (
        // Start a repetition:
        $(
            // Each repeat must contain an expression...
            $element:expr
        )
        // ...separated by commas...
        ,
        // ...zero or more times.
        *
    ) => {
        // Enclose the expansion in a block so that we can use
        // multiple statements.
        {
            let mut v = Vec::new();

            // Start a repetition:
            $(
                // Each repeat will contain the following statement, with
                // $element replaced with the corresponding expression.
                v.push(format!("{}", $element));
            )*

            v
        }
    };
}

fn main() {
    let s = vec_strs![1, "a", true, 3.14159f32];
    assert_eq!(s, &["1", "a", "true", "3.14159"]);
}
```

### repeat multiple metavariables in a single repetition


```rust
macro_rules! repeat_two {
    ($($i:ident)*, $($i2:ident)*) => {
        $( let $i: (); let $i2: (); )*
    }
}

repeat_two!( a b c d e f, u v w x y z );

// error : repeat_two!( a b c d e f, x y z );
```

## Fragment Specifiers
### item
- Items are entirely determined at compile-time, generally remain fixed during execution, and may reside in read-only memory
- 컴파일에 결정 됨
- read-only 메모리

- modules
- extern crate declarations
- use declarations
- **function definitions**
- **type definitions**
- struct definitions
- enumeration definitions
- union definitions
- constant items
- static items
- trait definitions
- implementations
- extern blocks

- not 

## expression
- An expression evaluates to a value
- evaluation
