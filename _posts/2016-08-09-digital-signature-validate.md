
---
layout: post
title:  Digital Signature Validation theory and implementation
tags: [DSignature] 
---

**format**
- Signature : pem  
- Certificate : x509  

**theory**  
1. [client] Find signature files(author, distributor1, ...)  
2. [client] Request to validate signature each files.  

3. [server] Validate signature files. (return result, *cert_info*)  
	3-1. base check  
	  3-1-1. pre-step  
	    3-1-1-1. make signature data by files  
	    3-1-1-2. parse signature data with SignatureReader and SIGNATURE_SCHEMA  
	    3-1-1-3. make certificate chain(x509) with signature data  
	    3-1-1-4. check certificate domain  
	  3-1-2. signature validate with xmlsec1  
	3-2. additional check (plug in)  	
4. [client] Save cert_info, and set priviledge level(only dist1)  



**implementation**  
1. SignatureData : signature.xml로 생성  
2. SignatureReader : #1과 SIGNATURE_SCHEMA를 가지고 #parsing  
  parsing : (ParseScheme <- SaxReader(libxml2 interface)와 <tag:x509Certificate...>로 수행  
  => parsing후 CertificateLoader에 의해 Certificate(x509; base64 encoding) 객체로 만듦  
  => Certificate객체를 Signature Data에 insert  
3. CertificateCollection으로 chain 생성 및 root certificate 추가  
4. Figerprint parsing후 인증서의 domain 확인 (revoked, public, platform...)  
5. subject certificate 인증서 유효성 검사 (time, ---)  
6. xmlsec1 context에 signature file 및 certificate(only root certificate) 추가  
7. xmlsec1에서 signature validate 실행. (어느정도 하는지 확인해야 함.)  
8. xmlsec1 context에서 reference set 가져온 것과 contentPath와 비교.  
9. OCSP check.  

---
Certificate.h -- X509(subject, issuer)

---

|before|after|improve|file cnt|file size|
|---
1|LibOAuth|75|64|15%|21|172K|
2|OAuth2|88|71|19%|32|212K|
3|SampleAccount|82|70|15%|25|172K|
4|FileManager|121|98|19%|40|333K|
5|Appcommon|104|82|21%|37|284K|
6|ResourceManagement|494|310|37%|297|2.8M|
7|ContextHistory|82|69|16%|23|164K|
8|MessagePort|76|69|9%|21|148K|
9|SyncManager|78|67|14%|21|156K|
10|Simplehome|75|66|12%|21|140K|




		before	after	improve	file cnt	file size  
1	LibOAuth	75	64	15%	21	172K  
2	OAuth2	88	71	19%	32	212K  
3	SampleAccount	82	70	15%	25	172K  
4	FileManager	121	98	19%	40	333K  
5	Appcommon	104	82	21%	37	284K  
6	ResourceManagement	494	310	37%	297	2.8M  
7	ContextHistory	82	69	16%	23	164K  
8	MessagePort	76	69	9%	21	148K  
9	SyncManager	78	67	14%	21	156K  
10	Simplehome	75	66	12%	21	140K  