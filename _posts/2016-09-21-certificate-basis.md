---
layout: post
title: Certificate basis and format
tags: [digital signature]
---

## [전자서명 (Digital Signature)][1]
> 전자서명이라 함은 **서명자를 확인**하고 서명자가 당해 전자문서에 **서명을 하였음**을  
> 나타내는 데 이용하기 위하여 당해 전자문서에 첨부되거나 논리적으로 결합된  
> 전자적 형태의 정보를 말한다.  + 전자 문서의 무결성을 보증.

- 즉, 서명자 인증, 부인 방지, 무결성 보장을 목적으로 함.
- **전자문서의 해시(HASH)값을 서명자의 개인키(전자서함명생성정보)로 변환(암호화)한 것**으로서  
  RSA사에서 만든 PKCS#7 의 표준이 널리 사용되고 있음.

### 서명부 (Signning)
1. (Digest) Hash 함수를 통해 전자 문서 A의 Hash 값을 구함. 
2. (Encryption) 이 Hash 값을 서명자의 Private Key 로 암호화함.
3. 암호화된 Hash값과 서명자의 public key 원본 전자 문서 A 끝에 첨부함.

### 검증부 (Verification)
1. 첨부된 Hash값을 제외하여 Hash 함수를 통해 Hash 값을 구함.
2. 그 다음 문서 뒤에 달린 암호화된 Hash값을 철수의 Public Key로 복호화함.
3. 복호화된 Hash값과 받은 문서의 Hash값과 비교함.

---
## [용어][3]
> SSL은 서버 인증(Server Authentication), 클라이언트 인증(Client Authentication)  
> 그리고 데이타 암호화(Data Encryption) 기능을 제공  
> **인증(Authentication)** 통신의 상대방이 맞는지 확인하는 절차를 의미  
> **암호화**는 데이타가 누출되더라도 외부에서 이 내용을 해독할 수 없게 하는 걸 의미  

## [X.509 인증서 (Certificate)][2]
> X.509는 암호학에서 공개키 인증서와 인증알고리즘의 표준 가운데에서 공개 키 기반(PKI)의 ITU-T 표준이다.

- 인증서는 Tree와 같은 구조를 이루면서 인증서끼리 서명을 하게 됨.
- Tree의 최상위 node 즉, root에 있는 인증서를 발행한 기관을 Root Certification Authority (Root CA)라 함.
- 이 Root CA는 자체 서명 (self signed) 되어 있고 기본적으로 이 root CA는 신뢰 할 수 있다고 판단.  

### X.509 v3 인증서 구조
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
...
- Certificate Signature Algorithm
- Certificate Signature

### X.509 인증서 확장자
>- .CER - CER 암호화 된 인증서. 복수의 인증서도 가능.
>- .DER - DER 암호화 된 인증서.
>- .PEM - (Privacy Enhanced Mail) **Base64로 인코딩** 된 인증서.
>  - "-----BEGIN CERTIFICATE-----"와 "-----END CERTIFICATE-----" 의 형식. 
>- .PFX - .p12 참조.
>- .P12 - PKCS#12, 공개 인증서와 **암호로 보호되는 개인 키**를 가질 수 있다(복수도 가능).  

> PKCS#12는 PFX(**개인 정보 교환**:Personal inFormation eXchange)이 발전된 형태이며,
  하나의 파일에서 공개 / 개인 자료들을 교환할 때 쓰인다.

---

## COMMON CERTIFICATE FORMATS

### PEM Format 
- It contains the **‘—–BEGIN CERTIFICATE—–” and “—–END CERTIFICATE—–”**며 statements.
- Several PEM certificates and even the Private key can be included in one file.
- But most platforms(eg:- Apache) expects the certificates and Private key to be in separate files.
- They are **Base64 encoded ASCII files**.
- extensions : .pem, .crt, .cer, .key
- Apache and similar servers uses PEM format certificates

## DER Format
- It is a **Binary form of ASCII PEM format** certificate.
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


[1]:https://www.google.co.kr/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&ved=0ahUKEwj7_veelZbRAhUN4mMKHbjJD3EQFggeMAA&url=https%3A%2F%2Fko.wikipedia.org%2Fwiki%2F%25EC%25A0%2584%25EC%259E%2590%25EC%2584%259C%25EB%25AA%2585&usg=AFQjCNEF8lnCfWrND41eP7kWRoG4Ncjs0A&sig2=zgr3FxNWR0p2L8OGCDKFjA&bvm=bv.142059868,d.cGc
[2]:https://ko.wikipedia.org/wiki/X.509
[3]:http://btsweet.blogspot.kr/2014/06/tls-ssl.html
