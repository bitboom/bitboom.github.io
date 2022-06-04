---
layout: post
title:  Digital Certificate
tags: [security]
---

#### [Public Key Certificate][1]
Public key의 owner를 증명하는 전자문서로 key, owner(subject), issuer 및 meta 정보를 포함한다.  
Digital certificate나 identity certificate라고도 불리며, email encryption, code siging, TLS에 사용 된다.  
대표적인 국제 표준으로 [X.509][2](.pem, .p12...)가 있다.  
Certificate는 Tree 구조를 이루면서 certificate끼리 서명을 하며, tree 구조의 최상위 node에 있는  
Certificate를 Root certificate라 한다. root certificate는 self-signed 되어 있으며 신뢰할 수 있다고 판단한다.

#### [App Signing and Certificates][3]
App signing은 application의 author를 확인하고 무결성을 검증하기 위해 사용되는 기술이다.  
Application을 packging 할 때 개발자(author)는 1) certificate(.p12)를 생성한 후 application을 서명한다.  
.p12 format의 경우 private-key가 certificate에 포함된 format이다. PKI의 구조에 따라 signature를 생성할 땐 private-key가 필요하기 때문이다.  
서명의 대상은 application내 모든 구성요소를 포함하며, 모든 구성요소의 1)hash값을 구한 후 그 값들을 모아 2)signature를 생성한다.  
그 결과물이 Tizen의 경우 author-signature.xml이다. Tizen은 distributor(store)의 인증도 진행 하기에 distributorN.xml의 인증서도 포함한다.

#####  App Signing Root Certificate
Root certificate의 경우 self-signed 되어있는 즉, 자신의 key로 서명된 certificate이다.  
해당 certificate는 신뢰할 수 있다고 판단하며 root certificate를 통해 개발자 certificate를 서명, 발급한다.  
Application 설치시 signature.xml을 검증하게 되는데 signature.xml에 존재하는 author-certificate는 image내에  
자신을 서명한 상위 certificate를 찾아가며 chain을 생성하며 최종 root certificate가 존재해야한다.  
해당 root certificate는 image 내에 .pem format으로 존재하게 된다. pem format은 p12 format과 다르게 private-key를 포함하지 않는다.

#### SSL Certificates
HTTPS를 통하여 웹서버가 신뢰할 수 있는 곳인지 알기 위하여 SSL을 이용하며 SSL은 내부적으로 certificate를 사용한다.  
SSL 통신의 단계 중 서버로부터 받은 certificate를 신뢰할 수 있는 root certificate로부터 서명된 것인지를 확인하기 위하여  
App signning와 같이 certificate chain을 생성하여야 하는데 이때 image 혹은 browser에 이미 root certificate를 가지고 있어야한다.

##### SSL Root Certificate
SSL 통신을 하기 위해 필요한 root certificate를 client는 이미 가지고 있어야 한다. brower의 경우 자체적으로 내장하고 있는 경우도 있지만   
운영체제 내 특정 디렉토리에 가지고 있으며 SSL 통신시 해당 디렉토리를 참조하여 root certificate를 찾는다.  
해당 root certificate를 못찾았을 시에 SSL 통신은 실패하게 된다.

#### Trust Anchor
Root certificate는 또 다른 말로 trust anchor라고 한다. 신뢰할 수 있는 지점이라고 하는 것인데,  
client는 실제 platform에 내장된 것이 아닌 것이 아닌 자신이 선택한 root certificate만 가능하도록 설정 할 수 있다.  
이 기능을 custom trust anchor라 한다.  

[1]:https://en.wikipedia.org/wiki/Public_key_certificate
[2]:https://en.wikipedia.org/wiki/X.509
[3]:https://developer.tizen.org/development/training/native-application/understanding-tizen-programming/application-signing-and-certificates-0
