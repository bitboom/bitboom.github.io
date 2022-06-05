---
layout: post
title: SGX Overview
tags: [security]
---

## Software Guard Extensions
운영체제는 권한 모델(커널모드, 유저모드)을 사용하여
자신만의 메모리 영역을 보호한다.  

SGX는 Skylake 이후
인텔 CPU칩에 추가된 보안 관련 명령어 셋으로,
이를 활용하면
유저 모드에서 동작하는
프로그램도
자신만 볼 수 있는
고유한 메모리 영역을
정의 할 수 있다.  

SGX가 보호하는
프로그램 내 메모리 영역을
엔클레이브(Enclave)라 한다.
엔클레이브에 저장된 데이터는
프로그램 자신만 볼 수 있다.
운영체제나 루트(root) 권한이 있는
프로세스도 볼 수 없다.

SGX의 위협 모델(Threat model)은
"엔클레이브를 제외한
모든 프로세스는
안전하지 않다."이다.
모든 프로세스에는
운영체제와 하이퍼바이저(hypervisor)도
포함 된다. SGX는 암호화를 사용하여
이 위협 모델로부터
프로그램을 보호한다.
엔클레이브는 CPU 내에서
동작할 때를 제외하고
항상 암호화 된 채로
존재한다.
따라서 외부에서는
엔클레이브의 암호화된 형태 밖에
볼 수 없다.
SGX는 부채널 공격(side channel attack)에 대한
보안성은 보장하지 않기 때문에 
프로그램은 자체적으로 부채널 공격에
안전하게 설계 되어야 한다.

SGX는 클라우드 컴퓨팅,
DRM(Digital Rights Management)와 같이
외부로부터 보호되어야 데이터가
있을 때 유용하도록 설계 되었다.

## SGX Local (Intra-Platform) attestation
Local Attestation은 Enclave들 사이에서
서로 같은 플랫폼 내 존재하는지 
증명하는 방법이다.

![Local Attestation](http://www.sgx101.com/wp-content/uploads/2017/09/Screen-Shot-2018-07-01-at-7.55.51-PM.png)

1-1. Enclave B는 자신의 MRENCLAVE 값을 Enclave A에게 보낸다.   
2-1. Enclave A는 Enclave B로부터 받은 B's MRECLAVE 값을 사용하여, B's REPORT 구조체를 하드웨어에 요청한다. (EREPORT)  
2-2. Enclave A는 B's REPORT 구조체를 Enclave B에게 보낸다.  
2-3. Enclave B는 Enclave A로부터 받은 REPORT 구조체를 하드웨어에게 검증하도록 요청한다. (EGETKEY)  
2-4. 검증이 완료되면 **Enclave B는 Enclave A가 자신과 같은 플랫폼에 있는 것이 증명된다**.  
3-1. Enclave B는 Enlave A로부터 받은 REPORT에서 A's MRENCLAVE를 이용하여 2의 과정을 반복 할 수 있다.  
3-2. 3-1의 과정을 통해 **Enclave A는 Enclave B가 자신과 같은 플랫폼에 있는 것이 증명된다**.  

* REPORT 교환시 구조체의 user-data에 디피 헬만 키 교환의 데이터를 사용해서 Secure Channel 생성 가능

## Keyword
- MRENCLAVE: Enclave를 식별하는 고유 값 (Software TCB라 불리는 Enclave Measurment의 결과 값)
- REPORT: Enclave의 정보를 담고 있는 구조체
- REPORT KEY: 플랫폼(CPU)에 속하는 Enclave들의 REPORT를 서명하기 위한 CPU 고유키
- EREPORT: REPORT
- EGETKEY: 하드웨어 루트키 2종으로 부터 5종의 세컨드-키를 파생시키는 명령어
	- 하드웨어 루트키 (H/W Secrets)
		- Root Provisioning Key (Intel know)
		- Root Sealing Key (Intel cannot know)
	- 세컨드-키 (Second derivation key)
		- Report Key (for all enclaves)
		- Seal Key (for all enclaves)
		- EINIT Token Key (only for SGX architectural enclaves)
		- Provision Seal Key (only for SGX architectural enclaves)
		- Provisioning Key (only for SGX architectural enclaves)

# References
- [Software Guard Extensions](https://en.wikipedia.org/wiki/Software_Guard_Extensions)
- [SGX 101 Local Attestation](http://www.sgx101.com/portfolio/local_attestation/)
- [Local (Intra-Platform) Attestation](https://software.intel.com/en-us/node/702983)
