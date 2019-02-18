---
layout: post
title: C&C View in SW Arch Document
tags: [architecture]
---

# Component in Software System

소프트웨어 아키텍처 문서(Software Architecture Document)에서 소프트웨어 시스템이 어떻게 동작하는지 표현하기 위해 C&C View (Component and Connector View)를 사용한다. 전체 시스템을 컴포넌트로 나누고 컴포넌트의 관계를 커넥터로 연결하여 동작을 설명 하는 것이다.

--- 

# 컴포넌트 (Component)
컴포넌트란 소프트웨어 시스템을 관련 있는 파트들로 나눈 것이다. 패키지가 될 수 있고 클래스가 될 수 있다. 더 추상적인 용어로도 사용 할 수 있으며 정의에 따른 제약은 없다. 시스템의 동작을 설명하기 위한 구성요소로 표현하면 된다.

컴포넌트는 인터페이스를 사용하거나 제공하는 두가지 방식으로 표현 가능하다.

![Component 250%](https://upload.wikimedia.org/wikipedia/commons/e/e8/Component.PNG)

'O'는 인터페이스를 제공하는 것이고, ')'는 인터페이스를 필요(사용)로 하는 것이다.

---

# 컴포넌트 다이어그램 (Componet Diagram)
C&C View는 UML의 컴포넌트 다이어그램(Component Diagram)으로 그릴 수 있다.
컴포넌트 다이어그램은 크게 세가지 요소로 표현한다.

- Component
- Provided Interface
- Require Interface

*(Draw.io의 UML TOOLBOX에서 해당 요소를 찾을 수 있다.)*

아래는 wikipedia에 나타나있는 Policy Administration System의 컴포넌트 다이어 그램이다.

![Component Diagram](https://upload.wikimedia.org/wikipedia/commons/thumb/b/b8/Policy_Admin_Component_Diagram.PNG/1280px-Policy_Admin_Component_Diagram.PNG)

가장 큰 컴포넌트로 표현된 Policy Admin은 IPolicyService와 IProductService를 제공한다. 또한 PolicyServer는 외부 컴포넌트 Document Management의 Document Management를 사용한다.

---

# 컴포넌트(Component)와 모듈(Module)
컴포넌트와 모듈을은 "시스템이 어떻게 동작하는가, Technical View"를 표현하는 것과 "시스템이 어떻게 구현되는가, Function View"를 표현하는 것으로 구분 할 수 있다. 

**두 개의 명확한 관계를 구분하는 것보다 중요한 것은 "표현이 시스템의 구조를 잘 설명할 수 있는가"이다.**

---

# Provided Interface(PI)와 Require Interface(RI)의 경계
컴포넌트 다이어그램을 그릴 때 PI와 RI는 정해져 있지 않다.  

예를 들어 MVC패턴에서 컨트롤러와 모델의 관계를 구성 할 때, 아래와 같이 Controller가 인터페이스를 제공하는 "Controller -O)- Model"의 구조가 될 수 있고.
```cpp
	Controller.setValue(Model.data);
```

또한 아래와 같이 Controller가 인터페이스를 사용하는 "Controller -(O- Model"의 구조가 될 수 있다.

```cpp
	Controller.Model.setValue();
```
