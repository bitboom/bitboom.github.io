---
layout: post
title: DSL with Jinja2
tags: [python]
---

본 글에선 C++의 구조체를 정의하는 DSL(Domain Specific Language)을 구현하기 위해 Jinja2를 사용하는 예를 설명한다. 이 예의 동작 방식은 Google의 Protocol Buf의 message 생성 방식이나 Osquery의 Table 생성방식을 간접적으로 이해 할 수 있다.

# Jinja2
> Jinja2 is a full featured template engine for Python.  

Jinja2는 파이썬으로 제작된 템플릿 엔진이다. 뼈대가 되는 템플릿 파일을 정의하고 수정할 부분을 파라미터로 넘겨줌으로써 최종 파일을 만들 수 있다.

Flask에서 동적 웹페이지를 만들기 위해 Jinja2를 사용하기도 하고, Osquery에선 DSL(Domain Specific Language)를 구현하기 위해 사용한다.

# I & O
임의로 정의한 *.struct 을 입력으로 하여 *.h 파일을 출력한다.
## Input: Socket.struct
```
struct_name("Socket")
members([
    Member(name="fd", type="int"),
    Member(name="address", type="std::string"),
])
```

## Output : Socket.h
```
#include <string>

namespace generated {

struct Socket {
  int fd;
  std::string address;
};

} // namespace generated
```

# #1 Define header file as template
Jinja2를 사용해 출력할 템플릿의 포맷을 정의한다. 위 아웃풋의 뼈대이다.
```python
HEADER_TEMPLATE = """
#include <string>

namespace generated {

struct {{name}} {
{% for member in members %}\
  {{member.type}} {{member.name}};
{% endfor %}\
};

} // namespace generated
"""
```

# #2 Parse input file & Match code
입력 파일을 읽어 들여서, Jinja2를 통해 렌더링하기 전에 입력 파일의 내용을 파이썬의 자료형에 매칭 시켜야한다.

```python
with open(filename, "r") as file_handle:
    tree = ast.parse(file_handle.read())
    exec compile(tree, "<string>", "exec")
```

위의 코드가 실행 되면 입력 파일의 각 행은 아래의 함수, 클래스에 매칭되어 실행 된다.


파일의 각 라인을 읽어들이는 것은 AST(abstract syntax tree)를 사용하고, 읽어들인 AST의 Node들을 각 함수에 매칭시키는 것은 'compile()'과 'exec()'함수를 사용한다. 두 함수 모두 파이썬 내장함수로 'compile()'은 소스를 코드로 변환시키는 것이고, 'exec()'는 변환된 코드를 실행 시키는 것이다.

동작의 설명은 복잡하지만, 하려고 하는 것은 간단하다. 입력 파일의 'struct_name'이 파이썬으로 정의한 함수'def struct_name'가 매칭 될 수 있도록 하는 것이다.


```python
class Member(object):
    def __init__(self, **kwargs):
        self.name = kwargs.get("name", "")
        self.type = kwargs.get("type", "")

def struct_name(name):
    print("Struct name is " + name + ".")
    struct.name = name

def members(member_list):
    print("Struct member size is " + str(len(member_list)) + ".")
    struct.members = member_list
```

# #3 Render template with Jinja2
#1에서 생성한 템플릿에 #2에서 뽑아낸 데이터를 파라미터로 넘겨주어 렌더링한다.
```python
jinja2.Template(HEADER_TEMPLATE).render(
            name=self.name,
            members=self.members
        )
```

---

# Full example
```python
# python gen-struct.py socket.struct
import ast
import jinja2
import os
import sys

HEADER_TEMPLATE = """
#include <string>

namespace generated {

struct {{name}} {
{% for member in members %}\
  {{member.type}} {{member.name}};
{% endfor %}\
};

} // namespace generated
"""

def usage():
    print("Usage: %s <filename>" % sys.argv[0])

class StructState:
    def __init__(self):
        self.name = ""
        self.members = []

    def generate(self):
        self.header_content = jinja2.Template(HEADER_TEMPLATE).render(
            name=self.name,
            members=self.members
        )

        base = os.path.dirname(os.path.realpath(__file__))
        self.header_path = os.path.join(base, "%s.h" % self.name)
        print("Generate header file to " + self.header_path + ".")

        with open(self.header_path, "w") as file_h:
            file_h.write(self.header_content)

struct = StructState()

class Member(object):
    def __init__(self, **kwargs):
        self.name = kwargs.get("name", "")
        self.type = kwargs.get("type", "")

def struct_name(name):
    print("Struct name is " + name + ".")
    struct.name = name

def members(member_list):
    print("Struct member size is " + str(len(member_list)) + ".")
    struct.members = member_list

def main(argc, argv):
    if argc < 2:
        usage()
        sys.exit(1)

    filename = argv[1]
    with open(filename, "r") as file_handle:
        tree = ast.parse(file_handle.read())
        exec compile(tree, "<string>", "exec")
        struct.generate()

if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
```
