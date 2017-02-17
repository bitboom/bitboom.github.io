---
layout: post
title: systemcall EINTR handling
tags: [linux]
---

## [시스템 호출 system call][1]
> 시스템 호출(system call)은 운영 체제의 커널이 제공하는 서비스에 대해,  
> 응용 프로그램의 요청에 따라 커널에 접근하기 위한 인터페이스.

> 시스템 호출이란 프로그래밍 언어에서 지원하지 않는 기능에 대하여  
> 운영 체제의 루틴을 호출하여 이용하는 것을 말한다.  
> 시스템 호출의 세 가지 기능은 다음과 같다.

> 1. 사용자 모드에 있는 응용 프로그램이 커널의 기능을 사용할 수 있도록 하는 것.
> 2. 시스템 호출을 부르면 사용자 모드에서 커널 모드로 바뀐다
> 3. 커널에서 시스템 호출을 처리하면 커널 모드에서 사용자 모드로 바뀌어 작업을 계속한다.


## 시스템 호출의 유형

- 프로세서 제어(process Control)
- 파일 조작(file manipulation)
- 장치 관리(Device Management)
- 정보 유지(Information maintenance)
- 통신(Communication)

## errno
systemcall을 사용했을 때 실행 중 error가 발생 했다면, 그 이유를 알아서 조치를 취해야 함.
=> 에러에 대한 발생 원인을 저장하는 변수 **errno**

### EINTR
errno 값 중 하나로 systemcall 수행 중 인터럽트가 걸려 수행이 중단 된 경우
따라서 해당 값이 리턴 된 경우 적절한 상태에서 다시 진행해야함.

### EINTR 처리
보통 인터럽트의 경우 자신과 상관 없는 경우가 많기 때문에
1. system call 값이 -1이 return 되는 경우 && EINTR 이면 다시 system call을 수행
2. signal 처리를 변경 -> sigaction에 SA_RESTART 옵션을 지정하여 자동적으로 계속 수행

```
	while (1) {
		auto descriptor = ::creat(path, mode);
		if (descriptor == -1) {
			if (errno == EINTR) {
				continue;
			}
			throw;
		}
		return;
	}
}
```

[1]:https://ko.wikipedia.org/wiki/%EC%8B%9C%EC%8A%A4%ED%85%9C_%ED%98%B8%EC%B6%9C
