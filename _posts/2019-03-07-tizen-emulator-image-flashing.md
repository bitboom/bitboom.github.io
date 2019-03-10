---
layout: post
title: Tizen emulator image flashing
tags: [tizen]
---

# Tizen Platform Image
타이젠의 플랫폼 이미지는 [download.tizen.org](http://download.tizen.org/snapshots/tizen/unified/latest/images/)에서 받을 수 있다. 현재 공식적으로는 Wearable과 Mobile 이미지를 공개하고 있다.

---

# Tizen Studio
타이의 IDE는 이클립스 기반으로 만들어졌으며 [developer.tizen.org](https://developer.tizen.org/development/tizen-studio/download)에서 받을 수 있다. 에뮬레이터를 실행하기 위해서 타이젠 스튜디오를 설치하여야 한다.

---

# Tizen Emulator Image Flashing
타이젠 SDK의 버전에 맞게 [에뮬레이터 이미지](http://download.tizen.org/snapshots/tizen/unified/latest/images/emulator/mobile-emulator32-wayland/)를 받은 후 아래와 같이 Flashing 할 수 있다.

1. 이미지 압축 해제
```sh
$ tar -xvzf tizen-unified_20190228.2_wearable-emulator-circle.tar.gz 
emulator-sysdata.img
emulator-rootfs.img
```

2. 이미지 파일 시스템 검사  
```sh
$ e2fsck -f emulator-rootfs.img 
e2fsck 1.42.9 (4-Feb-2014)
Pass 1: Checking inodes, blocks, and sizes
Pass 2: Checking directory structure
Pass 3: Checking directory connectivity
Pass 4: Checking reference counts
Pass 5: Checking group summary information
emulator-rootfs: 21087/80000 files (0.5% non-contiguous), 299293/306171 blocks
$ e2fsck -f emulator-sysdata.img 
e2fsck 1.42.9 (4-Feb-2014)
Pass 1: Checking inodes, blocks, and sizes
Pass 2: Checking directory structure
Pass 3: Checking directory connectivity
Pass 4: Checking reference counts
Pass 5: Checking group summary information
emulator-sysdata: 1198/8000 files (1.9% non-contiguous), 17490/25959 blocks
```

3. 이미지 파일 시스템 리사이징 (2G) = (1 block = 512 byte) * 4K  
```sh
$ resize2fs emulator-rootfs.img 2G
resize2fs 1.42.9 (4-Feb-2014)
Resizing the filesystem on emulator-rootfs.img to 524288 (4k) blocks.
The filesystem on emulator-rootfs.img is now 524288 blocks long.
$ resize2fs emulator-sysdata.img 2G 
resize2fs 1.42.9 (4-Feb-2014)
Resizing the filesystem on emulator-sysdata.img to 524288 (4k) blocks.
The filesystem on emulator-sysdata.img is now 524288 blocks long.
```

4. 타이젠 스튜디오 >> 에뮬레이터 매니저(Emulator Manager) >> CREATE >> '+' >> Image File >> 압축 해제한 폴더 입력
