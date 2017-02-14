---
layout: post
title: RPM packaging basis
tags: [linux]
---

## packaging flow
1. %prep : package.tar.gz unzip. to %builddir?
2. %build : make %builddir
3. %install : make %_buildroot
4. %files : owned by package
 
## term 
> BuildRequires:  
> A comma-separated list of packages required for building (compiling) the program.  
> These dependencies are not automatically determined, so you need to include everything needed to build the program.  

> Requires: A comma-separate list of packages that are required when the program is installed.  
> **Note that the BuildRequires tag lists what is required to build the binary RPM,**
> while the **Requires tag lists what is required when installing/running the program**

> *In many cases, rpmbuild automatically detects dependencies so the Requires tag is not always necessary.*  
> However, you may wish to highlight some specific packages as being required, or they may not be automatically detected.

- BuildRequires는 binary rpm을 build 할때 필요한 package list.  
- Requires는 실제 rpm이 install 되고 program이 실행 될 때 필요한 package list.    

## usage
- devel package는 실제 binary RPM에 들어 갈 필요가 없는 header, .pc, so symbol들이 포함된 pacakge
- devel pacakge는 기본적으로 원래의 package를 Requires에 추가함.

## Tip
 - package A가 package B를 BuildRequires에만 추가 했을 때, 즉 binary RPM을 만들 때만 필요할 때 주의
 - %file section (rpm이 설치 될 때 포함되는 파일들 영역)에서 BuildRequires의 macro 또는 file을 참조
 하지 않는지 주의. ex) TZ_SYS_SHARE  
 - pkgconfig(libX)는 libX의 pc파일을 가지고 있는 package를 찾아서 포함시켜주는 것으로 보임.


https://fedoraproject.org/wiki/Packaging:Guidelines 
https://fedoraproject.org/wiki/Packaging:PkgConfigBuildRequires
