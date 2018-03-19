---
layout: post
title:  Linux namespaces
tags: [linux]
---

### [Linux namespaces][1]
> Namespaces are a feature of the Linux kernel that partitions kernel resources 
> such that one set of processes sees one set of resources while another set of 
> processes sees a different set of resources.

네임스페이스는 커널 리소스를 분리하기 위하여, 리눅스에서 제공하는 기능이다.
커널 리소스를 분리한다는 것은 프로세스가 동작하는 동안 접근할 수 있는 리소스들에 대해 
제한 한다는 것인데 같은 네임스페이스에 속한 프로세스들은 해당 네임스페이스에 속한 
리소스에만 접근할 수 있다. 다른 네임스페이스에 속한 리소스들은 격리 되어 접근이 불가능하다.

네임스페이스는 각 프로세스들을 격리하는 기능을 제공함으로써 (접근 할 수 있는 리소스를 제한)
리눅스 컨테이너의 긴본적인 기술로 자리잡고 있다.

### Namespace kinds
리눅스 시스템은 단일(Single) 네임스페이스로 시작된다. 즉, 모든 프로세스들이 하나의 네임스페이스에 속해 있고 모든 리소스를 공유한다.  

프로세스들은 아래 타입들의 네임스페이스에 대해 추가적으로 생성 하거나 생성되어 있는 네임스페이스에 들어 갈 수 있다.

커널 4.1에는 7개의 종류의 네임스페이스를 제공하고 있다.
- Mount (mnt)
- Process ID (pid)
- UTS
- Network (net)
- IPC (ipc)
- User ID (user)
- Control group (cgroup)

#### Mount (mnt)
마운트 네임스페이스는 마운트 포인트를 제어한다. 마운트 네임스페이스가 생성되면 기존 네임스페이스의 마운트 
포인트들이 복사 되어 동일한 마운트 포인트를 가진다. 허나, 이 후 변경된 마운트 포인트는 각 네임스페이스에서만 유효하다.(Do not propagate.) 

변경할 마운트 포인트를 다른 네임스페이스에도 적용을 시키려면 Shared subtree를 이용하면 된다.

마운트 네임스페이스를 생성하려면 clone 시스템콜에 CLONE_NEWNS(New NameSpace) 플래그를 넘기면 되는데, 이는 
단순 마운트 네임스페이스를 위한 플래그이다. 최초 네임스페이스를 고안한 사람이 다른 네임스페이스를 예상하지 못하고
플래그 이름을 명명했다.

#### Process ID (pid)
리눅스에서 실행되는 모든 프로세스는 자신만에 고유한 PID를 부여 받는다. 커널은 프로세스들을 트리형태로 
관리하며 최상단에는 init 프로세스(PID: 1)가 존재한다.  

PID 네임스페이스는 각 네임스페이스에 속한 프로세스들에 대해 독립적인 PID를 부여할 수 있게 해주며, 
init 프로세스만이 가질 수 있는 PID를 가질 수 있게 해준다. PID 네임스페이스 내에서 최초 생성된 프로세스는 
PID 1번을 부여 받고 실제 init 프로세스처럼 여겨진다.  

**이로 인해 동일한 OS내에서 init 프로세스를 포함한 여러 프로세스가 PID 충돌 없이 컨테이너 내에서 실행될 수 있다.**

고아 프로세스(Orphaned process; 부모 프로세스가 종료 된 프로세스)들은 실제 init 프로세스에 붙여지게 되는데 
PID 네임스페이스 내에서도 똑같이 적용된다. 다시 말해 PID가 1인 init 프로세스가 종료 되면 같은 PID 네임스페이스 
내에 모든 프로세스들이 종료된다.

#### Network (net)
네트워크 네임스페이스는 네트워크 스택을 가상화한다. 이에 따라 각 네임스페이스는 IP 주소 셋, 라우팅 테이블, 소켓 리스닝과 
같은 리소스를 독립적으로 갖게 된다.

**이로 인해 동일한 포트로 다수의 서비스를 제공 할 수 있다.**

#### Interprocess Communication (IPC)
IPC 네임스페이스는 프로세스 간 통신을 환경을 격리한다. System V의 IPC나 POSIX의 메세지 큐가 대표적이고 기타 세마포어, 뮤텍스, 파일 락과 같은 자원에 대한 격리 기능을 제공한다.  

전형적으로 쉐어드 메모리를 분할하여 사용하기 위해 사용된다. 이를 통해 IPC를 이용한 DoS 공격에 대해 격리된 환경을 
제공한다.

#### Unix TimeSharing (UTS)
UTS 네임스페이스는 hostname과 domain-name에 대한 격리된 환경을 제공한다.  

이는 hostname에 종속적인 애플리케이션에대해 필수적인 기능을 제공한다.

#### User ID (user)
프로세스는 리소스에 접근하기 위하여 권한이 필요하며 접근 권한 제어(access control)를 위해 리눅스는 DAC을 
사용한다. 리소스에 uid, gui를 부여해 권한 있는 uid에 대한 접근만 허용하는데 uid가 0인경우 root를 의미하며 
강력한 권한을 갖는다.  

사용자 네임스페이스는 네임스페이스 간에  각기 다른 uid와 gid를 가질 수 있게 한다. 이는 생성된 네임스페이스(컨테이너) 
안에서는 uid 0을 갖도록하여 루트로 실행 될 수 있지만 실제 네임스페이 밖에서는 0이 아닌 일반 uid로 실행 되고 있음을 
뜻한다.

[1]:https://en.wikipedia.org/wiki/Linux_namespaces
