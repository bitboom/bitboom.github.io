---
layout: post
title: Implement 'ls', 'cat' using c++ on linux
tags: [c++]
---

```cpp

#include <cstdio>
#include <sched.h>

#include <iostream>
#include <string>
#include <memory>

std::string ls(const char *path)
{
	using FilePtr = std::unique_ptr<FILE, decltype(&::pclose)>;
	std::string cmd("/bin/ls ");
	cmd.append(path);

	FilePtr ls(::popen(cmd.c_str(), "r"), ::pclose);
	if (ls == nullptr)
		return std::string();

	std::vector<char> buf(1024);
	std::string ret;
	while (::fgets(buf.data(), buf.size(), ls.get()))
		ret.append(buf.data());

	return ret;
}

std::string cat(const char *path)
{
	using FilePtr = std::unique_ptr<FILE, decltype(&::pclose)>;
	std::string cmd("/bin/cat ");
	cmd.append(path);

	FilePtr ls(::popen(cmd.c_str(), "r"), ::pclose);
	if (ls == nullptr)
		return std::string();

	std::vector<char> buf(1024);
	std::string ret;
	while (::fgets(buf.data(), buf.size(), ls.get()))
		ret.append(buf.data());

	return ret;
}


```
