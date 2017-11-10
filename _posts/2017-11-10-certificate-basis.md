---
layout: post
title: 전자서명과 인증서
tags: [digital signature]
---

 우리는 집을 사기 위해 계약서를 쓸 때나 신용카드를 사용하고 나서 **서명**을 한다. 여기서 서명은 두 가지 효력이 있다.  
 
 첫째, 서명을 보고 서명을 한 사람(서명자)이 누군지 확인 할 수 있다. 둘째, 서명자가 나중에 딴소리 서명을 하였음을 나타내는데 사용한다. 예를 들어 은행에서 5000만원을 인출하기 위해 서명을 하고 나중에 500만원만 인출 했었다고 못하게 하는 것이다. 추가로 계약서에 서명 후 상대방이 임의로 계약서를 수정하지 못하도록 사본을 가지고 있거나 사진으로 기록에 남겨둔다.  
 
 결국 거래, 계약과 같은 상호간의 안전한 합의가 필요한 경우 서명이 사용 되며 서명은 아래의 세가지 보안적 요소를 만족 시켜야 한다.
-	인증 (Authentication)
-	무결성 (Integrity)
-	부인방지 (Non-repudiation)

### [전자 서명 (Digital signature)][1]
 전자 서명이란 실제 아날로그의 서명 방식을 전자 문서(파일, 메시지 등)에 사용하기 위하여 디지털화 시킨 것이다. 따라서 전자 서명은 해당 전자 문서에 대해 위의 세가지 보안 요소(인증, 무결성, 부인방지)를 보장한다. 
-	인증 (Authentication): 서명자의 공개키를 이용하여 서명 값을 검사
-	무결성 (Integrity): 전자 서명에 첨부된 해시 값을 검사
-	부인방지 (Non-repudiation): 서명자의 공개키를 이용하여 서명 값을 검사

### 전자 서명에 사용되는 알고리즘
 위의 세 가지를 보장 하기 위하여 기술적으로 공개키 기반 구조(Public Key Infrastructure)와 해시 함수가 사용되며, 사용 되는 알고리즘은 아래와 같다.

#### 서명 검증
1.	공개 키 쌍을 생성하는 키 생성 알고리즘
2.	개인 키를 사용하여 서명을 생성하는 알고리즘
3.	서명과 서명자의 공개 키를 사용하여 서명을 검증하는 알고리즘

#### 무결성 검증
1.	전자 문서의 해시 값과 서명 파일에 기록 된 해시 값을 비교하는 알고리즘

 전자 서명을 완전히 수행하기 위해선 전자 문서에 해당 서명 값과 공개 키를 붙여야 하는데, 이 때 공개 키를 붙이기 위하여 인증서가 사용 된다.

### [인증서 (Certificate)][3]
 인증서란 공개키 인증서(Public key certificate) 또는 전자 인증서(Digital certificate)로 불린다. 인증서는 **공개 키의 소유권을 확인 할 수 있도록 하기 위한 용도로** 만들어진 전자 문서이며 아래와 같은 내용이 포함된다. 
-	Subject: 해당 인증서를 발급 받은 대상 (즉, 소유주를 나타냄)
-	Issuer: 인증서를 발행한 대상
-	Key usage:  키의 사용 처 (ex. digital signature validation, key encipherment,  signing …)
-	Public key: 해당 Subject의 공개 키
-	Signature:  Issuer의 개인 키로 생성한 서명 값

### [X.509 인증서][4]
X.509 인증서는 공개키 인증서로 널리 사용 되는 ITU-T 표준이다. 각 인증서는 트리(Tree) 구조를 이루면서 상위 인증서(Issuer)에 의해 하위 인증서(Subject)가 서명 된다. 트리 구조에 최상위에 있는 인증서는 루트 인증서(Root certificate)라 칭하며 상위 인증서가 없기 때문에 자체 서명(self-signed)을 실시한다. 루트 인증서를 보유한 기관을 Root Certification Authority (Root CA)라 하며 기본적으로 Root CA는 신뢰 할 수 있다고 가정한다.

### X.509 인증서 확장자
 X.509 인증서는 다양한 확장자를 가지며 HTTPS 통신을 위한 SSL인증서는 경우 주로 PEM 확장자가 사용 된다.
- PEM: **Base64로 인코딩** 된 인증서
	("-----BEGIN CERTIFICATE-----"로 시작하며 "-----END CERTIFICATE-----" 로 끝남)
- DER: PEM 파일의 바이너리 포맷으로 된 인증서
- P12 - PKCS#12, 공개키 인증서와 **암호로 보호되는 개인 키**를 포함한 인증서
> PKCS#12는 PFX(**개인 정보 교환**:Personal inFormation eXchange)이 발전된 형태이며,
  하나의 파일에서 공개 / 개인 자료들을 교환할 때 쓰인다.

[1]:https://en.wikipedia.org/wiki/Digital_signature 
[2]:https://en.wikipedia.org/wiki/Public_key_infrastructure
[3]:https://en.wikipedia.org/wiki/Public_key_certificate
[4]:https://en.wikipedia.org/wiki/X.509
