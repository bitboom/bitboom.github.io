---
layout: post
title: Build-dependencies between multiple packages in RPM
tags: [framework]
---


Let's take a look at the following before proceeding.  

	- 'BuildRequires' tag is used to specify packages required at build time.
	- 'BuildRequires' has global visibility in the spec file.
	- 'Requires' tag is used to specify packages required at run time after once installed.
	- 'Requires' has local visibility at pacakge section in the spec file.
	- The {package}-devel is the package needed at build time to use the {package}.

# Background
There are packages with the following dependencies, 
A only needs B-devel package at build time.
```
A -> B -> C
```

---

So, A has to specify B-devel as 'BuildRequires' and B as 'Requires'.

```rpm
# In A Spec
%package A
BuildRequires: B-devel # or pkgconfig(B)
Requires: B
```

And B has to specify C-devel as 'BuildRequires' and C as 'Requires'.

```rpm
# In B Spec
%package B
BuildRequires: C-devel # or pkgconfig(C)
Requires: C
```

It's simple and clear.

---

# Problem
A caveat arises when B need to expose C's header to B's header. (For example, you might want to use the C's data type in B's header.)

```cpp
// In B's header,
#include <C/header.h>
```

Because A only sees B-devel at build time, it's a problem if B-devel doesn't have C's information.  

**rpm does not retrieve all relevant build information as we imagine.**

---

# Solution
So, B-devel has to specify C-devel as 'Requires' to tell A "You need C-devel if you want to build me". (Not BuildRequires)

```rpm
# In B Spec, (B needs to expose C's header)
%package B
BuildRequires: C-devel # or pkgconfig(C)
Requires: C

%package B-devel
Requires: B
Requires: C-devel
```

A still needs to look at B.
```rpm
# In A Spec
%package A
BuildRequires: B-devel # or pkgconfig(B)
Requires: B
```

That's all. But it is best not to make these dependencies.
