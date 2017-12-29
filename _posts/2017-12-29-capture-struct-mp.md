---
layout: post
title: Capturing struct member pointer
tags: [c++] 
---

ORM을 구현하기 위해선 struct의 member type, 정확히는 member type의 pointer를 저장할 수 있어야 한다. 아래는 template을 이용하여 struct의 member pointer를 저장하도록 구현한 예제이다.

```cpp
#include <iostream>

namespace type {

template<class O, class F>
struct Object {
	typedef F O::*Field;
	Field field;
};

template<class O, class T>
Object<O, T> capture(T O::*m) {
	return {m};
}

} // namespace type

struct Data {
	int i;
	bool b;
	std::string s;
};

int main() {
	auto c1 = type::capture(&Data::i);
	auto c2 = type::capture(&Data::b);
	auto c3 = type::capture(&Data::s);

	if (c1.field == &Data::i)
		std::cout << "[int] member captured." << '\n';

	if (c2.field == &Data::b)
		std::cout << "[bool] member captured." << '\n';

	if (c3.field == &Data::s)
		std::cout << "[string] member captured." << '\n';

	return 0;
}
```
