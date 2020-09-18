---
layout: post
title: Rust procedural macro (function-like)
tags: [rust]
---

In this post,
we will write 
rust source code
using proc macro.

## Procedural Macros
Procedural macros allow
creating syntax extensions
at compile time.
This means
it **allow you
to run code
at compile time**
that operates over Rust syntax.
It takes a piece of syntax and
produces a new bit of syntax.
(*from an AST to another AST*)

Procedural macros run during compilation,
and thus have the same powerful resources
that the compiler has.

There are three flavors.
- Function-like macros; custom!(...)
- Derive macro; #[derive(CustomDerive)]
- Attribute macro; #[CustomAttribute]

Procedural macros consists of
TokenStream input and TokenStream output.
[TokenStream](https://doc.rust-lang.org/stable/proc_macro/struct.TokenStream.html)
is a sequence of token trees
which is able to iterate.

## Function-like macros
Pro-macro should be defined
detached crate.
So, we create two crate
for example. (macro_derive, macro_main)

**We will create macro what
take function signature
and generate
the callable function.**

For that,
we have to parse 
the function signature
through macro input.

#### Syn crate
Rust *syn crate* provides
parser function.
We declare struct
to be parsed.
And parse macro's input
to the struct
using *syn crate*.

Function signature has
three component.
(function name, arguments, return type)
Let's parse signature
from function's syntax
with [Parse trait](https://docs.rs/syn/0.15.44/syn/parse/trait.Parse.html).

```rust
// Syntax: fn function(arguments...) -> return_type
// Signature member: function, arguments, return_t

type Function = Ident;
type Arguments = Vec<Type>;
type Return = Type;

struct Signature {
    function: Function,
    arguments: Arguments,
    return_t: Return,
}

struct Syntax {
    _fn_token: Token!(fn),
    ident: Function,
    _paren_token: token::Paren,
    paren_fields: Punctuated<Type, Token![,]>,
    _rarrow_token: Token!(->),
    return_t: Return,
}

impl Parse for Signature {
    fn parse(stream: ParseStream) -> Result<Self> {
        if stream.is_empty() {
            panic!("Write full function signature.");
        }

        let content;
        let syntax = Syntax {
            _fn_token: stream.parse().unwrap(),
            ident: stream.parse().unwrap(),
            _paren_token: parenthesized!(content in stream),
            paren_fields: content.parse_terminated(Type::parse).unwrap(),
            _rarrow_token: stream.parse().unwrap(),
            return_t: stream.parse().unwrap(),
        };

        Ok(Signature {
            function: syntax.ident,
            arguments: syntax.paren_fields.into_iter().collect(),
            return_t: syntax.return_t,
        })
    }
}
```

The core line is *stream.parse().unwrap()*.
It parse from ParseStream to speicific type.
And [Token!](https://docs.rs/syn/0.12.5/syn/macro.Token.html) macro 
is able to match
the specific token.
For parsing arguments,
it can be multiple types in parens,
use [parenthesize!](https://docs.rs/syn/0.15.39/syn/macro.parenthesized.html).

#### Quote crate
After parsing,
we need to generate
function.
Rust [quote crate](https://docs.rs/quote/1.0.7/quote/) provides
macro for turning Rust syntax tree data structures
into tokens of source code.

```rust
#[proc_macro]
pub fn make_function(input: proc_macro::TokenStream) -> proc_macro::TokenStream {
    let signature = syn::parse_macro_input!(input as Signature);
    let function = signature.function;
    let arguments = signature.arguments;
    let return_t = signature.return_t;

    if let 1 = arguments.len()  {
        let arg = &arguments[0];
        let tokens = quote!{
            fn #function(arg: #arg) -> #return_t {
                let ret: #return_t = arg * 2;
                println!("input {} * 2 = {}", arg, ret);
                ret
            }
        };
        tokens.into()
    } else {
        panic!("Invalid input");
    }
}
```

The core part is [quote!](https://docs.rs/quote/1.0.7/quote/macro.quote.html).
From parsed structures
we are able to
write rust source code using quote!.

That's done!.
Now we are use our proc-macro.

```rust
use macro_derive::make_function;

fn main() {
    make_function!(fn double(usize) -> usize);
    double(1); // 2
    double(2); // 4
    double(3); // 6
}
```

## Summary
Rust's proc-macro is powerful features
what are able to run code at compile time
and write code with macro.
(more powerful than c++ macros)
If we use proc-macro well,
we can improve performance
by running code at compile time.
Also, we can write code
by getting information
at the level of the compiler.
