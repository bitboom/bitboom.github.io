---
layout: post
title: Random access and sequential access
tags: [c++]
---

# Container
같은 타입의 객체들을 저장하기 위해서 STL은 컨테이너 클래스를 제공한다. 대표적인 컨테이너 클래스로 std::vector, std::list, std::map과 같은 것 들이 있다. 컨테이너에 저장된 객체들을 순회하기 위해서, 정확히는 접근 하기 위해서 알아야 할 개념이 있다. 

---

# Random access
std::vector는 객체들을 배열 형태로 저장한다. 즉, 메모리에 순차적으로 저장 된 형태이며 이는 임의적 접근(Arbitary Random Access)을 가능하게 한다. **임의적 접근이란 인덱스를 사용하여 바로 접근 할 수 있는 것을 뜻한다.** 순차적으로 저장되어 있기 때문에 배열에 저장된 객체의 크기와 인덱스를 사용해사 해당 객체의 위치를 계산 할 수 있기 때문이다. 

*operator[]와 at() 함수를 지원한다.*

---

# Sequential access
std::list는 객체들을 링크드 리스트 형태로 저장한다. 즉, 메모리에 각 객체를 위한 공간이 할당되고 이웃된 객체들 끼리만 위치를 알 수 있다. 따라서, n번째 저장된 객체에 접근하려면 n-1번까지의 객체를 순차적으로 접근해야 한다. 인덱스를 사용하여 접근 할 수 없다.

*operator[]와 at() 함수를 지원하지 않는다.*

---

# Sequence container
주의해야 할점은 std::vector와 std::list 모두 순서가 있는 시퀀스 컨테이너라는 것이다. 원소에 접근하기 위한 방식이 임의 접근과 순차적 접근으로 구분 될 뿐이다. STL이 지원하는 시퀀스 컨테이너는 아래와 같다.
- std::array (Random Access)
- std::vector (Random Access)
- std::deque (Random Access)
- std::forward_list (Sequential Access)
- std::list (Sequential Access)

---

# Iterator
STL에서는 컨테이너의 객체들을 순회하기 위하여 반복자의 개념이 있다. std::vector는 임의 접근을 하기에 임의 접근 반복자를 지원하고, std::list의 경우는 순차적 접근을 하기때문에 양방향 반복자를 지원한다.

- Random Access Iterator
- Bidirectional Iterator
- Forward Iterator

---

# Index of iterator
반복자의 인덱스를 구하는 방법으로는 두가지 방법이 있다.

- iterator - container.begin()
- std::distance(container.begin(), iterator)

첫번째는 현재 반복지에서 컨테이너의 처음을 가르키는 반복자를 빼서 계산하는 방식이고, 두 번째는 std::distance를 사용하여 첫 반복자 위치부터 현재 반복자 위치를 계산하는 것이다. 다시 말해, 첫번째 방법은 직접적으로 위치를 계산하여 값을 반환하고, 두번째는 순차적으로 움직여보면서 두 반복자를 비교하는 방식이다.  

첫번째 방법은 반복자가 operator-를 지원해야만 하는데, 이는 인덱스를 통한 객체 위치를 계산 할 수 있는 임의 접근 방식에서만 사용할 수 있다. 따라서 std::vector, std::array, std::deque에서 사용가능하다.  


두번째 방법은 반복자의 타입에 관계 없이 std::vector를 쓰던, std::list를 사용하던 구분 없이 사용할 수 있다.  

코드의 가독성과 성능으로는 첫번째 방법이 좋고, 재사용성면에서는 두번째 방법이 좋다. 실제 STL 구현상에서 std::distance도 최적의 방법을 채택하여 계산함으로 성능상 걱정은 하지 않아도 된다.

---

# Relation among iterators

임의 접근 반복자가 기본적인 반복자이다. 임의 접근 반복자로 바로 앞과 뒤의 원소를 접근 할 수 있기 때문에 양방향 반복자가 필요한 곳에 임의 접근 반복자가 사용 될 수 있다. 정방향 반복자 역시 임의 접근 반복자로 사용할 수 있다.

위 단락의 문장 자체가 해석하기 복잡한 부분이 있는데. 아래와 같이 전방향 반복자가 들어 가야 할 인자에 임의 접근 반복자를 사용 가능 하다는 뜻이다. 즉 std::vector로 아래의 함수를 사용 가능하다.

```cpp

template< class ForwardIt, class T >
constexpr ForwardIt lower_bound( ForwardIt first, ForwardIt last, const T& value );

```

--- 

# Reference
- http://www.cplusplus.com/reference/stl/
- https://www.geeksforgeeks.org/random-access-iterators-in-cpp/
- https://stackoverflow.com/questions/24147609/stl-what-is-random-access-and-sequential-access
- https://stackoverflow.com/questions/2152986/what-is-the-most-effective-way-to-get-the-index-of-an-iterator-of-an-stdvector
