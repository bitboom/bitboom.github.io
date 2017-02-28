---
layout: post
title: SIGPIPE handling
tags: [linux]
---

## SIGPIPE
- Linux network programming시 서버가 죽거나 어떠한 이유로 읽기가 되지 않는 프로세스에  
쓰려고 한다면(send()), 커널로부터 **SIGPIPE**의 signal을 받음
- SIGPIPE는 단순이 error를 return하는 것이 아니라 client 프로그램을 abort 시킴
- 따라서, signal을 hadling 해야함.   

## SIGPIPE handling
### signal 
단순 signal 함수로 간단하게 SIGPIPE를 무시하게 할 수 있지만 멀티 스레딩 환경에  
적합하지 않으므로 싱글 스레드가 확실히 보장될 때만 간편하게 쓸 수 있음.
```
signal(SIGPIPE, SIG,IGN);
```
### sigaction 
```
struct sigaction sig_act;
sig_act.sa_handler = SIG_IGN;
sigemptyset( &sig_act.sa_mask );
sig_act.sa_flags = 0;
sigaction(SIGPIPE, &act, NULL);
```

### send() FLAG : MSG_NOSIGNAL 
```
send(sockfd, buf, sizeof(buf), MSG_NOSIGNAL);
```

## error handling
위와 같은 방법으로 SIGPIPE로 인한 client의 abort를 막은 다음  
errno의 값을 통하여 pipe error를 확인 할 수 있음 (EPIPE)
