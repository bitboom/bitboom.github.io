---
layout: post
title: Linux Kernel Crypto API on User Space
tags: [sqlite]
---

# [Linux Kernel Crypto API](https://www.kernel.org/doc/html/v4.17/crypto/index.html)
> The kernel crypto API offers a rich set of cryptographic ciphers as well as other data transformation mechanisms and methods to invoke these. 

Crypto API는 리눅스 커널에서 사용하는 암호화 프레임워크이다. 커널 스페이스의 모듈이기 때문에 유저 스페이스에서 직접 사용할 수 없고 Netlink 소켓을 사용하여 통신하여야한다.

# AF_ALG
AF_ALG는 커널에서 제공하는 Netlink 소켓 타입이다. Openssl과 같은 많은 유저 스페이스의 암호화 라이브러리가 있음에도, AF_ALG가 제공 되는 이유는 다음과 같다.
- kernel mode / supervisor mode에서만 사용 가능한 하드웨어 가속
- 키와 같은 중요한 데이터와 민감한 파라미터들이 커널 메모리에 상주 (Openssl은 유저 스페이스 메모리)
- 유저 메모리 스페이스(Memory footprint) 효율
 
# User Space Interface Library - [libkcapi](https://github.com/smuellerDD/libkcapi)
 Crypto API를 유저 스페이스에서 사용가능하도록 하는 CAPI이다.

# User Space Practice written by C++
libkcapi를 사용하지 않고 C++에서 Crypto API를 사용하는 간단한 예제를 작성해보자.
## Message Digest with Crypto API (SHA256)
1. AF_ALG 소켓 생성 및 알고리즘 파라미터 전달
```cpp
struct sockaddr_alg sa; 
std::memset(&sa, 0, sizeof(sa));
sa.salg_family = AF_ALG;
//
std::string type = "hash";
snprintf((char *)sa.salg_type, sizeof(sa.salg_type),"%s", type.c_str());
//
std::string ciphername = "sha256";
snprintf((char *)sa.salg_name, sizeof(sa.salg_name),"%s", ciphername.c_str());
```
2. Netlink 소켓 연결 및 암호화 요청
```cpp
const int SHA256_DIGEST_SZ = 32;
std::vector<unsigned char> buf(SHA256_DIGEST_SZ);
//
std::string plain = "Plain txt.";
//
int sockfd = socket(AF_ALG, SOCK_SEQPACKET, 0);
::bind(sockfd, (struct sockaddr *)&sa, sizeof(sa));
//
int accpfd = accept(sockfd, NULL, 0);
::write(accpfd, plain.c_str(), plain.size());
::read(accpfd, buf.data(), SHA256_DIGEST_SZ);
//
::close(sockfd);
::close(accpfd);
```

3. 결과 값 16진수 출력
```cpp
std::cout << std::hex << std::setfill('0') << std::setw(2);
for (int i = 0; i < SHA256_DIGEST_SZ; i++)
	std::cout << static_cast<int>(buf[i]);
std::cout << std::endl;
```

## Full example
```cpp
// g++ sha256.cpp -std=c++11
#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>

#include <unistd.h>
#include <sys/socket.h>
#include <linux/if_alg.h>
#include <linux/socket.h>

int main(void)
{
	struct sockaddr_alg sa; 
	std::memset(&sa, 0, sizeof(sa));
	sa.salg_family = AF_ALG;

	std::string type = "hash";
	snprintf((char *)sa.salg_type, sizeof(sa.salg_type),"%s", type.c_str());

	std::string ciphername = "sha256";
	snprintf((char *)sa.salg_name, sizeof(sa.salg_name),"%s", ciphername.c_str());

	const int SHA256_DIGEST_SZ = 32;
	std::vector<unsigned char> buf(SHA256_DIGEST_SZ);

	std::string plain = "Plain txt.";

	int sockfd = socket(AF_ALG, SOCK_SEQPACKET, 0);
	::bind(sockfd, (struct sockaddr *)&sa, sizeof(sa));

	int accpfd = accept(sockfd, NULL, 0);
	::write(accpfd, plain.c_str(), plain.size());
	::read(accpfd, buf.data(), SHA256_DIGEST_SZ);

	::close(sockfd);
	::close(accpfd);

	std::cout << std::hex << std::setfill('0') << std::setw(2);
	for (int i = 0; i < SHA256_DIGEST_SZ; i++)
		std::cout << static_cast<int>(buf[i]);
	std::cout << std::endl;

	return 0;
}
```
