
## Exception Vector Table
운영체제 혹은 펌웨어 개발자는 제일 먼저 Vector Table을 설정한다.
Vector Table은 인터럽트가 발생 했을 때, Fault가 발생 했을 때, 시스템 콜이 호출 되었을 때 처리해야 할 핸들러를 담고 있다.
CPU는 이러한 익셉션들이 발생 했을 때 하드웨어 적으로 Vector Table을 참조하여 핸들러를 호출 해준다.

---

## VT with Processor Mode
Armv7 아키텍처는 [프로세서 모드](https://developer.arm.com/documentation/den0013/d/ARM-Processor-Modes-and-Registers)를
기반으로 Vector Table을 작성하였다. 
인터럽트가 발생하면 어떤 모드인지, 커널은 어떤 모드에서 실행 되는지 그리고 유저 프로그램은 어떤 모드에서 실행되는지는 프로세서 모드에 의해 결정 된다.

---

## VT with Exception Level
Armv8 아키텍처 이후로 [Exception Level](https://developer.arm.com/documentation/102412/0102)을 기반으로 Vector Table을 작성한다.
Armv8에는 보안 모드에서 동작할 수 있는 Trust Zone이 추가 되었고, CPU는 해당 모드를 지원했어야 했다.
Arm은 단순히 모드를 확장하는 것이 아니라 새로운 [Exception Level](https://developer.arm.com/documentation/102412/0102)을 도입하여아키텍ㅌ


이는 64-bit를 위한 aarch64에 해당하는 것이다. armv7의 호환성을 위해 aarch32는 armv7의 프로세서 모드를 유지한다.
__Exception Level은 Armv9에 추가된 CCA 또한 컴팩트하게 지원한다.

---


