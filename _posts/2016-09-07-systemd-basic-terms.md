---
layout: post
title:  systemd의 기본적인 용어 설명
tags: [systemd] 
---
## unit
아래 항목에 대해 정보를 가지고 있는 것을 unit configuration file이라고 한다.  
- service  
- socket  
- start-up target  
- device  
- e.t.c  

unit file에 적용 할 수 있는 공통적인 configuration option들은  
unit file 내에  [Unit]이나 [Install] section에 정의 할 수 있다.  
추가로 각각의 unit들은 specific한 section을 가질 수 있는데 service file의  
[service]같은 것이다.  

각 섹션 중 중요한 것을 알아보면,  
```
[Unit]  
Requires= # 다른 unit들과 dependency를 정의하는 부분.
          # 다른 unit이 activation이 실패하면 같이 실패 됨.

Wants=    # Requires와 같은 기능을 하지만 다른 unit이 activation 실패해도 
          # 영향을 받지 않음. 따라서 다른 unit이 start-up할 때 hooking 하여
          # 같이 실행 될 때 사용함.

Before=   # dependency 있는 unit들을 ordering하는 속성.
After=    # 만약 foo.service unit에 Before=bar.service를 설정하면
          # foo.service가 다 strat-up 될 때 까지 bar.service unit은 delay 됨.
          # 죽, 내 before=로 설정한 unit 전에 실행 시키라는 속성.

[Install] # systemd의 runtime에 interpret되는 것이 아니라 unit을 설치하는 
          # systemd enable/disable command에 의해 영향을 받는 section

WantedBy=   # 이 option을 설정하면 systemlctl enable시 .wants/ .requires/
RequiredBy= # 폴더에 symbolic link를 생성해줌


````

## socket
.socket 확장자를 갖는 unit configuration file로 아래와 같은 정보를 가지고 있다.
 - IPC  
 - network socket  
 - socket activation을 위해 위해 systemd에 의해 관리 되는 file system(FIFO) 

## Example
> `Allowing units to be enabled`  
> The following snippet (highlighted) allows a unit (e.g. foo.service)   
> to be enabled via systemctl enable:
>
> [Unit]  
> Description=Foo
>
> [Service]  
> ExecStart=/usr/sbin/foo-daemon>  
>
> [Install]  
> WantedBy=multi-user.target  
> After running systemctl enable,  
> a symlink /etc/systemd/system/multi-user.target.wants/foo.service  
> linking to the actual unit will be created.  
> It tells *systemd to pull in the unit when starting multi-user.target.*  
> The inverse systemctl disable will remove that symlink again.  

## TODO
1. 개발자 블로그 내용 추가.
2. systemctl start, enable 차이 추가.
3. socket, service, target unit 설명 추가.

## references
- https://www.freedesktop.org/software/systemd/man/systemd.unit.html  
- https://www.freedesktop.org/software/systemd/man/systemd.socket.html
- http://0pointer.de/blog/projects/inetd.html
- http://kateee.tistory.com/51
- http://unix.stackexchange.com/questions/47695/how-to-write-startup-script-for-systemd
