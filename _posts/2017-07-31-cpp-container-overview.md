---
layout: post
title:  C++ Container Overview
tags: [c++]
---

STL은 아래와 같이 크게 세가지로 구성되며 이 중에서 Container에 관해서 알아보자.

1. Container
2. Algorithm - 컨테이너들에 대해 실행 되는 알고리즘.
3. iterator - 컨테이너와 알고리즘을 연결시켜줌.

C++ 표준 라이브러리에는 다양한 컨테이너를 지원하며 크게 Sequential Container와 Associative Container로 나뉜다.
### Sequential Container
  Sequential container 각자 특화된 사용 시나리오가 있지만, 가장 유용하게 쓰이는 것은 std::vector이다.

- **std::vector => 크기가 동적으로 변하고, 메모리가 자동으로 관리, 성능 우수. (95% 이상 사용)**
- std::array => runtime에 크기가 조정되지 않는 유일한 컨테이너, 메모리 및 성능상 추가 부담을 최소화 할 때 사용.
- std::deque => std::vector와 유사하지만 컨테이너의 front에 추가를 할 때 사용.
- std::list => 임의의 위치에 대한 연산에 최적화, Doubly-linked List.
- std::forward_list => 임의의 위치에 대한 연산에 최적화, Singly-linked List.

### Associative Container
  Associative container은 키-값 쌍을 담는 컨테이너로 순서 있는(ordered) 연관 컨테이너와  
  순서 없는(unordered) 연관 컨테이너로 나뉜다.  둘의 가장 큰 차이는 성능이며,  
  순서 있는 연관 컨테이너의 접근 시간은 크기의 로그이 비례하지만,  
  순서 없는 연관 컨테이너의 경우 크기에 상관 없이 상수 접근 시간을 혀용한다.

#### Ordered Associative Container
- std::set => 연관된 값이 없고 동일한 자료형의 키만 저장, 중복 허용 X
- **std::map => 키-값 형태로 저장하며, 키가 정렬됨, 연관 컨테이너에서 가장 많이 사용**
- std::multiset => 중복된 키들을 허용
- std::multimap => 중복된 키들을 허용

#### Unordered Associative Container
- std::unordered_set
- std::unordered_map
- std::unordered_multiset
- std::unordered_multimap

### Iterator
  STL은 100개 이상의 알고리즘을 제공하며, 컨테이너의 모든 요소나 특정 범위에 대해 작용한다.  
  이 때 범위(range)는 두 개의 반복자(iterator)로 정의되는데, 첫 iterator는 range의 시작을 정의하고  
  두번째 iterator는 range의 끝을 정의한다. *이 때, end iterator는 범위의 끝을 하나 지나친 원소를 가리킨다.*



> 위의 내용은 [프로그래밍인사이트] 핵심 C++ 표준 라이브러리 책의 일부를 정리한 내용입니다.
