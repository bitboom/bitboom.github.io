---
layout: post
title: Binary Search Operations in STL Algorithm
tags: [c++]
---

# Search Algorithm
탐색 알고리즘에는 이진 탐색, 해시 탐색, 선형 탐색이 있다.

이중 STL에서 지원하는 이진 탐색의 Operation들을 살펴보자.
- binary_search
- lower_bound
- upper_bound
- equal_range

*이진 탐색의 특성상 컨테이너의 원소는 정렬 되어 있어야 한다.*

---

# std::binary_search
STL에서 지원하는 이진 탐색 함수이다. 컨테이너 내에 원하는 값이 있는지 없는지를 boolean 값으로 반환 한다.

```cpp
#include <iostream>
#include <vector>
#include <algorithm> // std::binary_search
 
int main()
{
	std::vector<int> data = { 1, 2, 3, 3, 5, 5, 6 };
	if (std::binary_search(data.begin(), data.end(), 4))
		std::cout << "Found" << std::endl; 
	else
		std::cout << "Not Found" << std::endl;

	if (std::binary_search(data.begin(), data.end(), 5))
		std::cout << "Found" << std::endl; 
	else
		std::cout << "Not Found" << std::endl;

	return 0;
}
```
```sh
Not Found
Found
```

---

# std::lower_bound
binary_search로는 해당 원소의 위치를 찾을 수 없다. 위치를 찾을 때 유용한 함수는 lower_bound이다. lower_bound는 범위 내에서 찾는 값보다 첫번째로 크거나 같은 값의 위치를 반환한다. 만약 해당 값을 찾지 못하면 두번 째 인자를 반환한다.

```cpp
#include <iostream>
#include <vector>
#include <iterator> // std::distance
#include <algorithm> // std::lower_bound
 
int main()
{
	std::vector<int> data = { 1, 2, 3, 3, 5, 5, 6 };
	auto lower = std::lower_bound(data.begin(), data.end(), 4);

	std::cout << "Index: " << std::distance(data.begin(), lower) << ", ";
	std::cout << "Value: " << *lower << std::endl;

	lower = std::lower_bound(data.begin(), data.end(), 7);
	if (lower == data.end())
		std::cout << "Cannot find." << std::endl;

	return 0;
}
```

```sh
Index: 4, Value: 5
Cannot find.
```

---

# std::upper_bound
upper_bound는 범위 내에서 찾는 값보다 첫번째로 큰 값의 위치를 반환한다. lower_bound와 마찬가지로 해당 값을 찾지 못하면 두번 째 인자를 반환한다.


```cpp
#include <iostream>
#include <vector>
#include <iterator> // std::distance
#include <algorithm> // std::lower_bound
 
int main()
{
	std::vector<int> data = { 1, 2, 3, 3, 5, 5, 6 };
	auto upper = std::upper_bound(data.begin(), data.end(), 5);
	std::cout << "Index: " << std::distance(data.begin(), upper) << ", ";
	std::cout << "Value: " << *upper << std::endl;

	return 0;
}
```
```sh
Index: 6, Value: 6
```

---

# std::equal_range
equal_range를 그대로 해석하면 찾고자 하는 값과 같은 값들의 범위를 반환 하는 것처럼 보이지만, 사실 lower_bound와 upper_bound의 std::pair를 반환한다. 역시 해당 값에 대한 결과를 찾지 못하면 두번 째 인자를 반환한다.

```cpp
#include <iostream>
#include <vector>
#include <iterator> // std::distance
#include <algorithm> // std::equal_range
 
int main()
{
	std::vector<int> data = { 1, 2, 3, 3, 5, 5, 6 };
	auto pair = std::equal_range(data.begin(), data.end(), 4);
	std::cout << "From " << std::distance(data.begin(), pair.first);
	std::cout << " To " << std::distance(data.begin(), pair.second) << std::endl;

	pair = std::equal_range(data.begin(), data.end(), 5);
	std::cout << "From " << std::distance(data.begin(), pair.first);
	std::cout << " To " << std::distance(data.begin(), pair.second) << std::endl;

	for (auto i = pair.first; i != pair.second; ++i)
		std::cout << *i << " ";
	std::cout << std::endl;

	pair = std::equal_range(data.begin(), data.end(), 7);
	if (pair.first == data.end() && pair.second == data.end())
		std::cout << "Cannot find." << std::endl;

	return 0;
}
```
```sh
From 4 To 4
From 4 To 6
5 5 
Cannot find.
```
