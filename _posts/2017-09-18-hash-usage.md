---
layout: post
title:  Hash의 용도
tags: [theory]
---

### [Hash Function][1]
 해시 함수(hash function)는 임의의 길이의 데이터를 고정된 길이의 데이터로 매핑하는 함수로  
해시 함수에 의해 얻어지는 값은 해시 값, 해시 코드, 해시 체크섬 또는 간단하게 해시라고 한다.  
 **Hash란 Hash function으로 부터 얻어지는 값으로 아래와 같은 용도로 사용 된다.**

1. Hash table
    - 사이즈가 큰 Data도 단순한 Hash 값으로 변환 후 비교
    - Data의 검색 속도 향상을 위해 사용 사용되는 자료구조

2. Encryption (Cryptography)
    - Hash Function의 특성 상 Hash 값으로 원본 메세지를 구하기 힘든 특성을 이용
    - 주로 Password와 같은 민감한 정보를 암호화하여 저장하기 위해 사용

3. [Data Integrity & Authentication of Message (HMAC, Cryptography)][2]
    - MAC(Message authentication code)이란, 메세지의 송신자를 인증하기 위한 정보로 tag라고도 불림
    - Hash를 기반으로 한 MAC을 [HMAC]라 함
    - HMAC 알고리즘을 위해 사전에 송수신자 간에 key가 공유 되어 있어야함
    - 암호학 해시 함수인 MD5나 SHA-1가 HMAC을 구하기 위해 사용 되며, 그 결과를 HMAC-MD5, HMAC-SHA1라 함.

> HMAC_MD5("key", "The quick brown fox jumps over the lazy dog")    = 80070713463e7749b90c2dc24911e275  
> HMAC_SHA1("key", "The quick brown fox jumps over the lazy dog")   = de7c9b85b8b78aa6bc8a7a36f70a90701c9db4d9  
> HMAC_SHA256("key", "The quick brown fox jumps over the lazy dog") = f7bc83f430538424b13298e6aa6fb143ef4d59a14946175997479dbc2d1a3cd8  
> ex) PKCS5_PBKDF2_HMAC_SHA1(openssl)

4. Network data checksum (Non-Cryptography)
    - CRC(순환 중복 검사, Cyclic Redundancy Check)는 데이터를 전송할 때 Data에 오류가 있는지를 확인하기 위해 사용
    - 구현이 쉽고, 데이터 전송과정에서 발생하는 흔한 오류를 검출하는데 탁월
    - 데이터의 무결성을 검사하는데는 사용할 수 없음

> CRC-16, CRC-32, CRC-64, CRC-128, CRC-256

ref) [Hash를 사용한 안전한 password 관리][4]


[1]:https://ko.wikipedia.org/wiki/%ED%95%B4%EC%8B%9C_%ED%95%A8%EC%88%98
[2]:https://en.wikipedia.org/wiki/Hash-based_message_authentication_code
[3]:https://ko.wikipedia.org/wiki/%EC%88%9C%ED%99%98_%EC%A4%91%EB%B3%B5_%EA%B2%80%EC%82%AC
[4]:http://d2.naver.com/helloworld/318732
