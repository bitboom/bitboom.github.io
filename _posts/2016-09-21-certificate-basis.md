---
layout: post
title: Certificate basis and format
tags: [digital signature]
---

<Digital Signature>
전자서명이라 함은 서명자를 확인하고 서명자가 당해 전자문서에 서명을 하였음을
나타내는 데 이용하기 위하여 당해 전자문서에 첨부되거나 논리적으로 결합된
전자적 형태의 정보를 말한다.

- 전자문서의 해시(HASH)값을 서명자의 개인키(전자서명생성정보)로 변환(암호화)한
것으로서 RSA사에서 만든 PKCS#7 의 표준이 널리 사용되고 있다.

* 방법 *

- Signning
1. (Digest) 원래의 문서 내용을 A라고 하면 A의 해쉬 값을 잘 알려진 Hash 함수인 SHA1 
같은 함수 하나를 정해 이런 Hash함수로 문서 A의 Hash 값을 구하고

2. (Encryption) 이 Hash 값을 보내는 사람(철수)의 Private Key 로 암호화 한다.
이런 다음 이렇게 암호화된 Hash값을 원래 문서 A 끝에 첨부하여 이 문서 전체를 받는
사람(영희)에게 보낸다.

- Verification
1. 영희는 먼저 메시지가 오면 해쉬값을 뺀 앞부분의 문서에 대해 철수가 사용했던
Hash함수를 이용해 받은 문서의 Hash값을 구한다.

2.그 다음 문서 뒤에 달린 암호화된 Hash값을 철수의 Public Key로 복호화한 다음 이
복호화된 Hash값을, 받은 문서의 Hash값과 비교하게된다. 
이 두개의 Hash값이 동일한 값이면 서명이 올바른 것이고 
값이 서로 다르거나 변환에 오류가 있으면 서명이 틀린 것이다.

< RSA >
RSA는 공개키 암호시스템의 하나로, 암호화뿐만 아니라 전자서명이 가능한 최초의 알고리즘으로
알려져 있다. RSA가 갖는 전자서명 기능은 인증을 요구하는
전자 상거래 등에 RSA의 광범위한 활용을 가능하게 하였다.

-----------------------------------------------------------------------------------------------

< Certificate >

- 인증서 소유자의 e-mail 주소
- 소유자의 이름
- 인증서의 용도
- 인증서 유효기간
- 발행 장소
- Distinguished Name (DN)
- Common Name (CN)
- 인증서 정보에 대해 서명한 사람의 디지털 ID
- Public Key
- 해쉬(Hash)

- 기본 원칙
SSL의 기본 구조는 당신이 인증서를 서명한 사람을 신뢰한다면, 서명된 인증서도 신뢰할 수 있다는 것이다.
이것은 마치 트리(Tree)와 같은 구조를 이루면서 인증서끼리 서명하게 된다. 그러면 최상위 인증서는?
이 인증서를 발행한 기관을 Root Certification Authority(줄여서 Root CA)라고 부르며,
유명한 인증 기관(역주:Verisign, Thawte, Entrust, etc)의 Root CA 인증서는 웹브라우저에 기본적으로 설치되어 있다.
이러한 인증 기관은 자신들이 서명한 인증서들을 관리할 뿐만 아니라 철회 인증서(Revoked Certificate)들도 관리하고 있다.
그러면 Root CA의 인증서는 누가 서명을 했을까? 모든 Root CA 인증서는 자체 서명(Self Signed)되어 있다.

< Refer >
https://injustfiveminutes.com/2013/12/02/pem-der-p7bpkcs7-pfxpkcs12-certificates-and-conversions/


--------------------------------------------------------------------------------------------

https://ko.wikipedia.org/wiki/X.509

< X.509 >
- X.509는 암호학에서 공개키 인증서와 인증알고리즘의 표준 가운데에서 공개 키 기반(PKI)의 ITU-T 표준이다.
X.509 시스템에서 CA는 X.500 규약에 따라 서로 구별되는 공개키를 가진 인증서를 발행한다.

한 조직의 인증된 루트 인증서는 그 PKI 시스템을 사용하는 모든 직원들에 분배될 수 있다.
인터넷 익스플로러나 모질라, 오페라와 같은 브라우저는 SSL 인증서라 불리는 미리 설치된 루트 인증서가있다.
사용자가 이 루트 인증서를 제거하거나 사용중지할 수도 있기는 하지만, 거의 그러지는 않는다.

X.509는 또한 CRL (certificate revocation list) 구현을 위한 표준도 포함한다.
IETF에서 승인된 인증서 유효성 점검 방법은 OCSP(Online Certificate Status Protocol)이다.

인증서의 구조
X.509 v3의 디지털 인증서의 구조는 아래와 같다.

Certificate
- Version 인증서의 버전을 나타냄
- Serial Number CA가 할당한 정수로 된 고유 번호
- Algorithm ID
- Issuer
- Validity
- Not Before
- Not After
- Subject
- Subject Public Key Info
- Public Key Algorithm
- Subject Public Key
- Issuer Unique Identifier (Optional)
- Subject Unique Identifier (Optional)
- Extensions (Optional)
- *Certificate Signature Algorithm
- Certificate Signature

- X.509 인증서 확장자

.CER - CER 암호화 된 인증서. 복수의 인증서도 가능.
.DER - DER 암호화 된 인증서.
.PEM - (Privacy Enhanced Mail) Base64로 인코딩 된 인증서.
 "-----BEGIN CERTIFICATE-----"와 "-----END CERTIFICATE-----" 가운데에 들어간다.
.P7B - .p7c 참조.
.P7C - PKCS#7 서명 자료 구조(자료는 제외), 인증서이거나 CRL(복수도 가능).
.PFX - .p12 참조.
.P12 - PKCS#12, 공개 인증서와 암호로 보호되는 개인 키를 가질 수 있다(복수도 가능).

- PKCS#7는 데이터를 서명하거나 암호화 할 때 쓰이는(enveloping) 표준이다.
  서명된 데이터를 검증할 때는 인증서가 필요하기 때문에 서명 자료 구조에 인증서를 포함하기도 한다.
  .P7C 파일은 데이터가 아니라 서명 자료 구조일 뿐이다.

- PKCS#12는 PFX(개인 정보 교환:Personal inFormation eXchange)이 발전된 형태이며,
  하나의 파일에서 공개 / 개인 자료들을 교환할 때 쓰인다.

- A .PEM 파일은 단수, 혹은 복수의 인증서와 개인 키를 가질 수 있으며
 적절한 시작/종료 라인 사이에 위치한다(CERTIFICATE or RSA PRIVATE KEY).


--------------------------------------------------------------------------------------------

https://injustfiveminutes.com/2013/12/02/pem-der-p7bpkcs7-pfxpkcs12-certificates-and-conversions/

< COMMON CERTIFICATE FORMATS >

< PEM Format > 
- It contains the ‘—–BEGIN CERTIFICATE—–” and “—–END CERTIFICATE—–” statements.
- Several PEM certificates and even the Private key can be included in one file.
- But most platforms(eg:- Apache) expects the certificates and Private key to be in separate files.

- They are Base64 encoded ASCII files.
- extensions : .pem, .crt, .cer, .key
- Apache and similar servers uses PEM format certificates

< DER Format >
- It is a Binary form of ASCII PEM format certificate.
- All types of Certificates & Private Keys can be encoded in DER format
- They are Binary format files
- extensions : .cer & .der
- DER is typically used in Java platform


NOTE: Only way to tell the difference between PEM .cer and DER .cer is to open the file 
      in a Text editor and look for the BEGIN/END statements.

< P7B/PKCS#7 >
- They contain “—–BEGIN PKCS—–” & “—–END PKCS7—–” statements.
- It can contain only Certificates & Chain certificates but not the Private key.
- They are Base64 encoded ASCII files
- extensions : .p7b, .p7c, .p7s
- Several platforms supports it. eg:- Windows OS, Java Tomcat

< PFX/PKCS#12 >
- They are used for storing the Server certificate, any Intermediate certificates & Private key in one encryptable file.
- They are Binary format files
- extensions : .pfx, .p12
- Typically used on Windows OS to import and export certificates and Private keys

---
