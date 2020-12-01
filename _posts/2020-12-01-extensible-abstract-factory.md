---
layout: post
title: Extensible abstract factory pattern, c++
tags: [c++]
---

This post introduces
**extensible abstract factory pattern**.
It is assumed that
you know the basic factory pattern and CRTP pattern.
*The sample code is written by c++14.*

**The main disadvantage of
abstract factory pattern is extensibility.**
Whenver`product` is added,
both `factory` and `product` have to be added respectively.

## Basic abstract factory pattern
Factory is only responsible for creating products.
But we need to add when product is added.
This mean the factory is lack of extensibility.  

And there are no reusable code.
If we need to apply abstract pattern to other product type,
we should write code from a to z.

```cpp
// Product
struct Button {
	virtual ~Button() = default;
	virtual void draw(void) = 0; 
};

struct LinuxButton final : Button {
	void draw(void) override { std::cout << "Linux button" << std::endl; }
};

struct MacOSButton final : Button {
	void draw(void) override { std::cout << "MacOS button" << std::endl; }
};

// Factory
struct ButtonFactory {
	virtual ~ButtonFactory() = default;
	virtual std::unique_ptr<Button> create(void) = 0;
};

struct LinuxFactory final : ButtonFactory {
	virtual ~LinuxFactory() = default;

	std::unique_ptr<Button> create(void) override
	{
		return std::make_unique<LinuxButton>();
	}
};

struct MacOSFactory final : ButtonFactory {
	virtual ~MacOSFactory() = default;

	std::unique_ptr<Button> create(void) override
	{
		return std::make_unique<MacOSButton>();
	}
};

int main() {
	std::unique_ptr<ButtonFactory> factory;
	std::unique_ptr<Button> button;
	
	factory = std::make_unique<LinuxFactory>();
	button = factory->create();
	button->draw();

	factory = std::make_unique<MacOSFactory>();
	button = factory->create();
	button->draw();

	return 0;
}
```

## Extensible abstract factory pattern
The purpose of this pattern takes both reusability and extensibility.  

For this, 
1. Couple factory and product with nested struct.
2. Make factory reusable with CRTP pattern.

Code is simple.

#### Reusable factory
```cpp
// abstract-factory.hpp
template <typename P>
struct AbstractFactory {
	virtual std::unique_ptr<P> create(void) = 0;
};

template <typename F, typename P>
struct Factory : AbstractFactory<P> { // Apply CRTP
	virtual ~Factory() = default;

	struct Product : P { // Couple factory and product
		virtual ~Product() = default;
	};

	std::unique_ptr<P> create(void) override {
		return std::make_unique<typename F::Product>(); // Generate concrete product
	};
};
```

#### Practice
For using extensible abstract pattern, just two step needed. 
1. Define product struct
2. Implement Concrete Product in Concrete Factory

```cpp
#include "abstract-factory.hpp"

// Product
struct Button {
	virtual ~Button() = default;
	virtual void draw(void) {}; 
};

// Factory #1
struct LinuxFactory : Factory<LinuxFactory, Button> {
	struct Product : Button {
		virtual ~Product() = default;

		void draw(void) override { std::cout << "Linux button" << std::endl; }
	};
};

// Factory #2
struct MacOSFactory final : Factory<MacOSFactory, Button> {
	struct Product : Button {
		virtual ~Product() = default;

		void draw(void) override { std::cout << "MacOS button" << std::endl; }
	};
};

int main() {
	std::unique_ptr<AbstractFactory<Button>> factory;
	std::unique_ptr<Button> button;
	
	factory = std::make_unique<LinuxFactory>();
	button = factory->create();
	button->draw();

	factory = std::make_unique<MacOSFactory>();
	button = factory->create();
	button->draw();

	return 0;
}
```
