---
layout: post
title: Logger level 1
tags: [c++]
---

```cpp

#include <iostream>
#include <sstream>

#define LOG (log(Logger()))

class Logger {
public:
	Logger() {}
	~Logger() { std::cout << stream.str() << std::endl; }

	Logger(Logger&& l) {
		stream << l.stream.str();
	}

	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete; 
	Logger& operator=(Logger&) = delete;

	// TODO Change to template.
	Logger& operator<<(const std::string& msg)
	{
		stream << msg;
		return *this;
	}

	friend Logger&& log(Logger&& logger)
	{
		return std::move(logger);
	}

private:
	std::ostringstream stream;
};

int main() {
	LOG << "Log test" << " " << "1";
	LOG << "Log test" << " " << "2";
	LOG << "Log test" << " " << "3";
	LOG << "Log test" << " " << "4";
	LOG << "Log test" << " " << "5";
	return 0;
}

```
