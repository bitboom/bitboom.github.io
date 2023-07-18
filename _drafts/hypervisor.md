이 글은 Arm 아키텍처 기반 가상화를 대상으로 한다.

## Hypervisor (가상화 소프트웨어) 는 하나의 하드웨어에 여러 운영체제를 동작 시킨다.
운영체제는 하나의 CPU Core에서도 가상화 방법들을 이용하여 여러 애플리케이션을 동작 시킨다.
Hypervisor는 같은 환경에서 여러 운영체제들을 동작 시키기 위해 고안 되었다.

## Hypervisor는 두 종류로 분류 된다.

Hypervisor는 Host OS의 유무에 따라 두 종류로 분류 할 수 있다.
Type 1 Hypervisor는 하드웨어 위에 Host OS없이  Bare-metal SW 형태로 동작하며 Guest OS들을 동작시킨다.
Type 2 Hypervisor는 하드위에 위에 Host OS의 애플리케이션 형태로 동작하여 Guest OS들을 동작시킨다.

- Type 1: Bare-metal Hypervisor
- Type 2: Hosted Hypervisor

Type 1 Hypervisor의 예로 Xen, Type 2 Hypervisor의 예로 KVM이 있다.
대부분의 하드웨어에서 Host OS가 동작 하기 때문에 Type 2는 호환성이 좋은 장점이 있다.
하지만 Host OS Layer가 추가되기 때문에 Type 1 보다 성능이 낮을 수 밖에 없는 심각한 단점이 있다. 
이를 개선하기 위해 Arm은 VHE를 이용하여 KVM이 EL2에서 직접 실행 될 수 있게 하였다.

## KVM은 Linux Kernel의 기능을 재활용 한다.
운영체제가 프로세스들을 가상화하는 것처럼, Hypervisor는 운영체제들을 가상화한다.
운영체제는 프로세스들을 스케쥴링하고, Hypervisor는 운영체제들을 스케쥴링한다.
이때 사용 되는 스케쥴링 코드를 재활용하면 효과적일 것이다.
그래서 나온 것이 Linux Kernel들의 핵심 기능들을 재활용한 KVM이다.

## Hypervisor의 필요한 기능들은?
운영체제는 프로세스들이 자신들이 CPU를 독점하면서 사용하는 것처럼 환상을 주기 위해
프로세스 전환시 CPU Context들을 저장 & 복원한다.
이처럼 Hypervisor는 운영체제 각각이 자신만이 하드웨어를 독점하는 환상을 제공해 주어야 한다.
그를 위해선 아래의 기능들이 필요하다.

- 운영체제 동작 모드보다 더 권한 있는 모드: Exception Level 2
- Core Context & State를 변환시키는 Exceptions에 대해 Trapping
- Exceptions과 Virtual Interrupt를 Routing
- Hypervisor가 Guest OS들을 isolate 하기 위한 Two-stage memory translation
- Hypervisor Call

## Virtualization History
Armv7에서 처음 Virtualization이 도입 되었으며,
Armv8에서는 Normal World에서만 Virtualization이 가능했다.
Armv8.4부터 Secure World Virtualization이 제공 되었으며,
Armv9엔 Realm Wolrd가 도입 됨에 따라 Hypervisor가 동작 할 수 있는 State는 3가지가 되었다.
