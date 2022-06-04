---
layout: post
title: Analysis XML Digital Signature shema
tags: [security]
---

Tizen platform내 어플리케이션의 무결성을 검증 및 개발자 인증을 위해 사용하는 
Digital Signature 기술은 XML Digital Signature 이다.

전반적인 signature형태는 아래 [XML Signature Example][1] 과 같다.

## Signature Shema (Tizen version)
```
<Signature Id="MyFirstSignature" xmlns="http://www.w3.org/2000/09/xmldsig#"> 
    <SignedInfo>  
    <CanonicalizationMethod Algorithm="http://www.w3.org/2010/xml-c14n2"/> 
    <SignatureMethod Algorithm="http://www.w3.org/2001/04/xmldsig-more#rsa-sha256"/> 
    <Reference Uri=> 
        <Transforms Algorithm="http://www.w3.org/2010/xmldsig2#transform">
        // 확인 <CanonicalizationMethod Algorithm="http://www.w3.org/2010/xml-c14n2"/>
        <DigestMethod Algorithm="http://www.w3.org/2001/04/xmlenc#sha256"/> 
        <DigestValue>dGhpcyBpcyBub3QgYSBzaWduYXR1cmUK...</DigestValue> 
    </Reference> 
    </SignedInfo>
    <SignatureValue>...</SignatureValue>
    <KeyInfo>
        <KeyValue>
        </KeyValue> 
   </KeyInfo>
   <Object ID>
</Signature>
```
## Signature Node  
- Signature : XML의 root node로 SignedInfo와 SignatureValue, KeyInfo를 담고 있다.
- CanonicalizationMethod : 정규화 알고리즘을 명시한다. 현재 C14N2가 필수 사용 알고리즘이다.
    - 정규화 알고리즘 : XML Signature 파일이 논리적으로는 동일하나 저장 되는 물리적 요소에 따라  
    다른 형태로 존재 할 수 있기 때문에, *논리적으로도 동일한 XML 파일을 물리적으로도 동일하게 변환  
    시키도록 하는 XML 정규화(Canonical XML) 알고리즘*을 사용한다.
- SingatureMethod : 서명 요소를 생성할때와 검증할 때 사용하는 서명 알고리즘을 명시한다.
    - Tizen platform에서는 rsa-sha256을 사용한다.
- Reference : 서명 객체 즉, 애플리케이션 내 파일들을 나타내며 URI로 위치를 작성한다.
- Transform : 서명이 되었는지 확인 하기 위한 용도와, data들을 정규화된 octet stream으로 변환 하는  
정규화 알고리즘을 명시한다.
- DigestMethod : 서명 객체들의 무결성을 보장하는 Hash, Digest 알고리즘을 명시한다.
    - Tizen platform에서는  MD5를 사용한다.
- DigestValue : Digest 알고리즘으로 Encoded된 값을 명시한다.
- KeyInfo : 서명에 확인 할 공개키의 정보를 담고 있다.
- Object : XML문서 내에 서명(Signature)가 포함 된 경우 Object 엘리먼트와 Id 속성을 통해  
원본 문서를 기술하며, 이를 참조하기 위해 Reference Node의 URI 속성에 Object Node의 Id 속성을 명시한다.
    > The [Object's Id][2] is commonly referenced from a Reference in SignedInfo,  
    > or Manifest. This element is typically used for enveloping signatures  
    > where the object being signed is to be included in the signature element.  
    > The digest is calculated over the entire Object element including start and  
    > end tags. 

[1]: <https://www.w3.org/TR/xmldsig-core2/#sec-o-Simple-2.0>
[2]: <https://www.w3.org/TR/xmldsig-core2/#sec-Object>
