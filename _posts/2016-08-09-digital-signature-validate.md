
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
-----------------------------


----


		before	after	improve	file size	references
1	LibOAuth	75	64	15%	172K	4
2	OAuth2	88	71	19%	212K	9
3	SampleAccount	82	70	15%	172K	8
4	FileManager	121	98	19%	333K	17
5	Appcommon	104	82	21%	284K	14
6	ResourceManagement	494	310	37%	2.8M	115
7	ContextHistory	82	69	16%	164K	6
8	MessagePort	76	69	9%	148K	4
9	SyncManager	78	67	14%	156K	4
10	Simplehome	75	66	12%	140K	4
11	MediaContent	738	395	46%	6.9M	7
12	Media	2660	1363	49%	26M	36
13	Player	735	395	46%	6.8M	8
14	MetadataExtractor	719	386	46%	6.7M	5
15	ThumbnailUtil	720	386	46%	6.7M	5
						
		before	after	improve	file size	references
1	LibOAuth	73	65	11%	172K	4
2	OAuth2	76	67	12%	212K	9
3	SampleAccount	80	65	19%	172K	8
4	FileManager	110	84	24%	333K	17
5	Appcommon	98	78	20%	284K	14
6	ResourceManagement	448	266	41%	2.8M	115
7	ContextHistory	77	64	17%	164K	6
8	MessagePort	70	61	13%	148K	4
9	SyncManager	72	62	14%	156K	4
10	Simplehome	70	61	13%	140K	4
11	MediaContent	774	394	49%	6.9M	7
12	Media	2663	1364	49%	26M	36
13	Player	736	382	48%	6.8M	8
14	MetadataExtractor	719	383	47%	6.7M	5
15	ThumbnailUtil	719	384	47%	6.7M	5
						
		before	after	improve	file size	references
1	LibOAuth	73	65	11%	172K	4
2	OAuth2	81	67	17%	212K	9
3	SampleAccount	77	65	16%	172K	8
4	FileManager	111	85	23%	333K	17
5	Appcommon	95	76	20%	284K	14
6	ResourceManagement	437	272	38%	2.8M	115
7	ContextHistory	76	65	14%	164K	6
8	MessagePort	71	61	14%	148K	4
9	SyncManager	72	62	14%	156K	4
10	Simplehome	70	61	13%	140K	4
11	MediaContent	742	395	47%	6.9M	7
12	Media	2669	1367	49%	26M	36
13	Player	733	393	46%	6.8M	8
14	MetadataExtractor	718	386	46%	6.7M	5
15	ThumbnailUtil	718	388	46%	6.7M	5
						
		before	after	improve	file size	references
1	LibOAuth	72	61	15%	172K	4
2	OAuth2	81	66	19%	212K	9
3	SampleAccount	77	65	16%	172K	8
4	FileManager	110	86	22%	333K	17
5	Appcommon	93	73	22%	284K	14
6	ResourceManagement	441	271	39%	2.8M	115
7	ContextHistory	74	63	15%	164K	6
8	MessagePort	70	60	14%	148K	4
9	SyncManager	73	61	16%	156K	4
10	Simplehome	69	56	19%	140K	4
11	MediaContent	739	392	47%	6.9M	7
12	Media	2665	1365	49%	26M	36
13	Player	738	390	47%	6.8M	8
14	MetadataExtractor	721	382	47%	6.7M	5
15	ThumbnailUtil	724	383	47%	6.7M	5
						
						
	signature 2					
		before	after	improve		
1	FileManager	167	115	31%		
2	OAuth2	124	98	21%		
3	Media	3995	1399	65%		
						
						
	signature 3					
		before	after	improve		
1	FileManager	226	146	35%		
2	OAuth2	167	124	26%		
3	Media	5329	1439	73%		
						
						
	signature 4					
		before	after	improve		
1	FileManager	280	177	37%		
2	OAuth2	207	156	25%		
3	Media	6660	1477	78%		
						
						
	signature 5					
		before	after	improve		
1	FileManager	354	209	41%		
2	OAuth2	254	184	28%		
3	Media	8035	1517	81%		
						
						
	signature 6					
		before	after	improve		
1	FileManager	412	243	41%		
2	OAuth2	301	215	29%		
3	Media	9328	1550	83%		
						
						
	signature 7					
		before	after	improve		
1	FileManager	457	274	40%		
2	OAuth2	324	239	26%		
3	Media	10660	1589	85%		
						
	signature 8					
		before	after	improve		
1	FileManager	516	298	42%		
2	OAuth2	375	270	28%		
3	Media	11993	1621	86%		
						
						
	signature 9					
		before	after	improve		
1	FileManager	571	327	43%		
2	OAuth2	414	300	28%		
3	Media	13369	1655	88%		
