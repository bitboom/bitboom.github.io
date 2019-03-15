---
layout: post
title: Factory Method Pattern with Modern c++
tags: [c++]
---

본 글은 팩토리 메소드 패턴의 내용를 정리하고, 템플릿 메소드 패턴을 같이 활용함으로써 복합 패턴을 정리한다. 이를 통해 객체 지향 설계의 OCP 원칙과 DIP 원칙을 준수 할 수 있다. 또한, C++의 템플릿 키워드를 적용하여 위 복합 패턴의 단점을 개선한다.

# Framework with Abstract Class
추상 클래스 간의 관계를 통하여 프레임워크의 동작을 정의 할 수 있다. 이 후 추상 클래스의 파생 클래스를 정의함으로써 프레임워크를 완성시킨다.

프레임워크의 동작 중 **확장 가능한 하위 클래스 생성 부분**에 팩토리 메소드 패턴을 적용 할 수 있다.  

예를 들어, MS 워드를 사용하여 워드 문서를 생성 할 수도 있고 같은 방식으로 PDF 문서도 생성 할 수 있는데, 이 때 적용 가능한 패턴이다.

---

# Factory Method Pattern
> [팩토리 메소드 패턴](https://ko.wikipedia.org/wiki/%ED%8C%A9%ED%86%A0%EB%A6%AC_%EB%A9%94%EC%84%9C%EB%93%9C_%ED%8C%A8%ED%84%B4)는 부모(상위) 클래스에 알려지지 않은 구체 클래스를 생성하는 패턴이며,  
> 자식(하위) 클래스가 어떤 객체를 생성할지를 결정하도록 하는 패턴이기도 하다. 

팩토리 메소드 패턴은 생성하고자 하는 객체를 추상 클래스로 정의하고, 추상 클래스의 수정 없이 파생 클래스를 확장 가능하도록 만든 패턴이다. 기본적인 추상 클래스 동작 방식에 클래스 생성의 규칙을 더한 것으로 보면 된다. 이로써 객체 지향 설계의 OCP원칙을 지킬 수 있다.  

# Factory Method Structure

![Class Diagram](https://upload.wikimedia.org/wikipedia/commons/thumb/a/a3/FactoryMethod.svg/1024px-FactoryMethod.svg.png)

[위키피디아](https://upload.wikimedia.org/wikipedia/commons/thumb/a/a3/FactoryMethod.svg/1024px-FactoryMethod.svg.png)에 나와 있는 클래스 다이어 그램은 팩토리 메소드의 가장 기본적인 것 표현 한 것이다. 이 구조만으로는 팩토리 메소드 패턴의 장점을 전부 사용 할 수 없다.  

결국, 하위 클래스를 생성하는 부분이 들어가야 하는데, 보통 파라미터에 하위 클래스를 나타 내는 인자를 더해서 Creator 함수에 추가한다.
```cpp
std::shared_ptr<Product> Creator::Create (ProductId id) {
    if (id == PRODUCT1) return std::make_shared<PRODUCT1>();
    else if (id == PRODUCT2) return std::make_shared<PRODUCT2>();
    ...	
}
```
위와 같이 Creator를 정의하면 Creator는 하위 클래스인 ConcreteCreator에 간접적인 디펜던시가 생긴다. 이는 DIP원칙을 지킬 수 없게 된다. 하위 클래스가 추가 되면 상위 클래스 안에 프로덕트 생성 부분의 변경이 필요하기 때문이다.

---

# Factory Method with Template Method
팩토리 메소드 패턴에 [템플릿 메소드 패턴](http://bitboom.github.io/template-method-with-cpp11)까지 적용하면 DIP 원칙까지 지킬 수 있게 된다.

**DIP 원칙을 따르기에 하위 모듈의 변경이 상위 모듈에 영향을 미치지 않는 구조를 완성 할 수 있다.**

*TODO: Factory Method + Template Method Class Diagram 추가*

---

# Factory Method Pattern Implementation Tatic
1. 팩토리 메소드를 가지는 Creator 클래스와 Creator 클래스에서 생성 할 Product 클래스를 정의한다.
2. Creator 클래스에 팩토리 메소드를 호출 할 템플릿 메소드를 추가한다.
3. ConcreteCreator 클래스와 ConcreteProduct 클래스를 구현한다.
4. ConcreteProduct가 많을 경우 ConcreteCreator를 템플릿 키워드를 이용하여 StandardCreator로 변환 한다.

---

# Define Creator Class and Product Class
PDF와 워드 문서를 생성하는 것으로 패턴의 예제를 구현해보자. 우선 문서와 문서를 생성하는 클래스의 관계를 추상 클래스로 설정한다.

```cpp
class Document {
public:
	virtual void info() = 0;
};

class DocumentCreator { 
public:
	// template method
	virtual std::shared_ptr<Document> create() final
	{
		return this->factory(); 
	}

protected:
	// factory method
	virtual std::shared_ptr<Document> factory() = 0;
};
```

DocumentCreator::create()는 템플릿 메소드 패턴을 적용하여, 하위 클래스에서 재정의 하지 못하게 한다. DocumentCreator::factory()는 팩토리 메소드 패턴을 적용하여 파생 클래스에서 객체 생성 부분을 재정의 하도록 한다.

# Define ConcreteCreator, ConcreteProduct Class
Document 클래스는 PDF, 워드로 확장한다. DocumentCreator 클래스도 그에 맞추어 병렬적으로 확장한다.

```cpp
class PDFDoc final : public Document {
public:
	void info() override
	{
		std::cout << "PDF Document." << std::endl;
	}
};

class WordDoc final : public Document {
public:
	void info() override
	{
		std::cout << "Word Document." << std::endl;
	}
};

class PDFDocCreator final : public DocumentCreator {
protected:
	virtual std::shared_ptr<Document> factory()
	{
		return std::make_shared<PDFDoc>();
	}
};

class WordDocCreator final : public DocumentCreator {
public:
	virtual std::shared_ptr<Document> factory()
	{
		return std::make_shared<WordDoc>();
	}
};
```
--- 

# Testing
추상클래스(Document, DocumentCreator)의 변화 없이 PDF, 워드를 생성 할 수 있다.

```cpp
int main() {
	std::shared_ptr<DocumentCreator> creator = std::make_shared<PDFDocCreator>();
	auto pdf = creator->create();
	pdf->info();

	creator.reset(new WordDocCreator);
	auto word = creator->create();
	word->info();

	return 0;
}
```
```sh
PDF Document.
Word Document.
```

# Improve factory method pattern with C++ template keyword
지금 구조는 Product가 추가 되면 ProductCreator가 병렬적으로 추가 되어 전체적인 클래스가 많아 진다. 

이 부분은 C++의 템플릿 키워드를 사용하여 표준 Creator를 정의함으로써 개선 할 수 있다. 
아래는 템플릿 키워드를 이용하여 정의한 StandardCreator의 코드이다.

```cpp
template <typename Product>
class StandardCreator final : public DocumentCreator {
protected:
	virtual std::shared_ptr<Document> factory() override
	{
		return std::make_shared<Product>();
	}
};

int main() {
	std::shared_ptr<DocumentCreator> creator = std::make_shared<PDFDocCreator>();
	auto pdf = creator->create();
	pdf->info();

	creator.reset(new WordDocCreator);
	auto word = creator->create();
	word->info();

	creator.reset(new StandardCreator<MarkDownDoc>);
	auto markdown = creator->create();
	markdown->info();

	return 0;
}
```
