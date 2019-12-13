---
layout: post
title: Pimpl idiom with modern C++
tags: [c++]
---

# [Pimpl idiom](https://docs.microsoft.com/en-us/cpp/cpp/pimpl-for-compile-time-encapsulation-modern-cpp?redirectedfrom=MSDN&view=vs-2019)
> The _pimpl idiom_ is a modern C++ technique to hide implementation, to minimize coupling, and to separate interfaces. Pimpl is short for "pointer to implementation."

The main point of the pimpl idiom is to hide the implementation, especially in member variables and private methods.

To hide public method implementation is not difficult problem. We just expose the method signature to header and write the implementation to the cpp file.

But, how to hide the member variables in header? Then, we can use pimpl idiom.  

For example, Let's say we need std::vector as member variable and we do not want to expose it to the header like below.
```cpp
class API {
private:
  std::vector<std::string> data; // we want to hide it from header.
};
```

We can hide it to the implementation and only expose the pointer. Let's walk through by code.

##  Basic example
The header exposes only one member variable as pointer(4-bytes). So the client-side compiler of this API does not need to know other information.
```cpp
// api.hpp
#include <memory>

class API { 
public:
	API();
	~API(); // Do not use define in header. also 'default'.
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;
};
```
The member variable is hided to library. (std::vector)
```cpp
// api.cpp
// $ g++ --std=c++1y -fPIC -shared api.cpp -o libapi.so
#include "api.hpp"
#include <std::vector>

class API::Impl {
private:
	// hide member variables and methods
	std::vector<int> data;
};

API::API() : pImpl(std::make_unique<Impl>())
{}
API::~API() = default;
```
The important thing is to declare API's destructor to header and define it to cpp file. If not, the client-side compiler of this library know only how to destroy API instance but cannot know how to destroy class API::Impl in API's destructor.  

Simply words, there is no code to destroy API:Impl in ~API().  

If we  delegate the destructor to cpp, the destory's logic is in library and the client-side compiler does not need to know how to destroy it.

Try to move destructor to header. You may meet below error.
```sh
$ g++ --std=c++1y -Wl,-rpath,./ -L./ main.cpp -lapi
/usr/include/c++/7/bits/unique_ptr.h:76:22: error: invalid application of ‘sizeof’ to incomplete type ‘API::Impl’
  static_assert(sizeof(_Tp)>0,
```

##  Advanced example
What if we have to use 'template constructor'? Template instance is instantiated when it is called. So, the client-side compiler makes it's concrete constructor. (Above example makes it by library-side compiler.)

Now the client-side compiler has to know how to destroy API::Impl correspond with concrete constructor. Because if exception is occurred, the concrete constructor called it.

The work around solution is wrapping API::Impl's destructor.
```cpp
#include <memory>

class API { 
public:
	API();
	~API();

	template <typename T>
	explicit API(T value)
	{ // do something }

private:
	class Impl;
	struct ImplDeleter // Add custom deleter of Impl
	{
		void operator()(Impl*);
	};

	std::unique_ptr<Impl, ImplDeleter> pImpl;
};
```

See, the declaration of ImplDeleter above and the definition of it below.

```cpp
#include "api.hpp"

#include <vector>

class API::Impl {
private:
	std::vector<int> data;
};

API::API() : pImpl(new Impl)
{
}

API::~API() = default;

void API::ImplDeleter::operator()(Impl* ptr)
{
	delete ptr;
};
```

Now the client-side compiler can delegate the way how to destroy API::Impl to library.

#### [Reference - stackoverflow](https://stackoverflow.com/questions/9954518/stdunique-ptr-with-an-incomplete-type-wont-compile)
