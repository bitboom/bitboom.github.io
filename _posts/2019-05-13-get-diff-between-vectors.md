---
layout: post
title: Get diff between vectors
tags: [c++]
---

---

# [Vector](http://www.cplusplus.com/reference/vector/vector/)
> Vectors are sequence containers representing arrays that can change in size.

std::vector는 stl에서 많이 쓰이는 컨테이너 중 하나다. std::vector간에 diff를 stl 함수에 대해 알아보자.

--- 

# [std::set_difference](https://en.cppreference.com/w/cpp/algorithm/set_difference)
std::set_difference는 <algorithm>에 있는 함수로 두 std::vector간에 diff를 출력 vector에 담아준다.  
사용법은 아래와 같이 간단하다.  

```cpp
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
 
int main() {
    std::vector<int> v1 {1, 2, 5, 5, 5, 9};
    std::vector<int> v2 {2, 5, 7};
    std::vector<int> diff;
 
    std::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), 
                        std::inserter(diff, diff.begin()));
 
    for (auto i : v1) std::cout << i << ' ';
    std::cout << "minus ";
    for (auto i : v2) std::cout << i << ' ';
    std::cout << "is: ";
 
    for (auto i : diff) std::cout << i << ' ';
    std::cout << '\n';
}
```
```sh
1 2 5 5 5 9 minus 2 5 7 is: 1 5 5 9
```
---

## Pre-condition
다만 해당 함수를 호출하기 전에 std::vector의 원소가 정렬 되어 있어야 한다. std::vector를 정렬하는 방법은 여러가지가 있는데 여기서 두가지 방법을 알아보자.

### [std::sort](http://www.cplusplus.com/reference/algorithm/sort/)
> Sorts the elements in the range [first,last) into ascending order.  

std::sort는 정렬하고자 하는 컨테이너 자체에 변경사항을 반영하기 때문에 원본 컨테이너를 보존하기 위해서는 복사를 진행하고 정렬을 진행한다.

---

### [std::multiset](http://www.cplusplus.com/reference/set/multiset/)
> Multisets are containers that store elements following a specific order, and where multiple elements can have equivalent values.  

원본의 컨테이너를 보존하기 위해 복사를 진행하고 정렬을 해야할 경우 std::multiset은 좋은 대안이 될 수 있다. std::multiset은 중복된 요소를 정렬된 형태로 저장하기 때문에 std::vector를 std::multiset에 인풋으로 넣는 것만으로 정렬된 컨테이너를 얻을 수 있다.

따라서, 원본을 보존하면서 두 std::vector의 diff를 구하는 방법은 아래와 같다.

1. 원본의 두 std::vector으로 std::multiset을 만든다.
2. std::set_difference의 인자로 (1)의 결과 값을 넣는다.

## Code snippet in open-source

위와 방식으로 diff를 구하는 오픈소스(facebook/osquery)의 코드조각은 아래와 같다. QueryData와 DiffReaults는 는 std::vector이다.

```cpp
DiffResults diff(const QueryData& old_, const QueryData& new_) {
  DiffResults r;
  QueryData overlap;

  for (const auto& i : new_) {
    auto item = std::find(old_.begin(), old_.end(), i);
    if (item != old_.end()) {
      overlap.push_back(i);
    } else {
      r.added.push_back(i);
    }
  }

  std::multiset<Row> overlap_set(overlap.begin(), overlap.end());

  std::multiset<Row> old_set(old_.begin(), old_.end());

  std::set_difference(old_set.begin(),
                      old_set.end(),
                      overlap_set.begin(),
                      overlap_set.end(),
                      std::back_inserter(r.removed));

  return r;
}
```
