---
layout: post
title: Arm CCA introduction
tags: [cca]
---

Confidential Computing, Arm CCA, Realm

## [Arm CCA Introduction](https://developer.arm.com/architectures/architecture-security-features/confidential-computing)
Arm사는 Arm9-A 아키텍처의 핵심 요소로 Arm CCA(Confidential Computing Architecture)를 공개했습니다.
CCA의 목적은 마이크로-프로세서가 들어가는 모든 요소에 Confidential Computing을 지원하는 것입니다.
Confidential Computing이란 나의 모든 데이터와 코드가 어디에서든, 누구로부터든 보호되며 Computing이 됨을 의미합니다.

이상적으로 들리지만 무슨 말인지 정확히 이해하기 어렵습니다.

우리는 넷플릭스와 유투브를 사용하여 미디어를 시청합니다.
넷플릭스는 나의 결제수단을 포함한 시청이력과 같은 개인정보를 유지, 관리해야 합니다.
보안상의 이유로 개인정보가 유출된다면 고객의 신뢰를 잃을 뿐더러, 심각한 상황에 빠질 수 있습니다.
하여 자신들의 웹서비스와 어플리케이션의 보안을 높이기 위해 많은 노력을 합니다.

그것만으론 충분 하지 않습니다. 고객은 TV와 스마트폰을 통해서 넷플릭스의 서비스를 이용합니다.
웹서비스는 클라우드 위에서 동작하고, 애플리케이션은 스마트폰 위에서 동작하지요.
넷플릭스가 아마존의 클라우드를 사용한다고 가정했을 때, 넷플릭스는 아마존의 클라우드를 신뢰 할 수 있을까요?
어떻게 넷플릭스는 안드로이드, 아이폰 위에서 동작했을 때 자신의 서비스가 보호 됨을 신뢰 할 수 있을까요.

클라우드, 안드로이드가 해킹 당해도 넷플릭스의 데이터와 코드는 보호되어야 합니다.
즉, Confidential Computing 기술은 넷플릭스의 서비스가 어디서 동작하던 보호되어 동작할 수 있게 합니다.

다시 첫 문단을 살펴보겠습니다.
> Arm사는 Arm9-A 아키텍처의 핵심 요소로 Arm CCA(Confidential Computing Architecture)를 공개했습니다.
> CCA의 목적은 마이크로-프로세서가 들어가는 모든 요소에 Confidential Computing을 지원하는 것입니다.
> Confidential Computing이란 나의 모든 데이터와 코드가 어디에서든, 누구로부터든 보호되며 Computing이 됨을 의미합니다.

## Confidential Computing
데이터는 모든 시점에서 보호 되어야합니다.
- at rest (stored in flash memory by a database)
- in motion (traversing a network)
- in use (being processed)

암호화는 `at rest`와 `in motion` 시점에 데이터를 보호하는 데 사용됩니다. 그런데 결국 `in use`시점엔 복호화가 되죠.

우리는 체크카드로  비밀번호를 걸어 둡니다. 강도에게 통장을 뺏겨도 비밀번호를 알려주지 않으면, 강도는 돈을 가져갈 수 없습니다.
만약 강도가 비밀번호까지 알아채고 은행에서 인출을 시도했을 때 어떻게 막을 수 있을까요?
내 통장으로 돈을 뽑을 수 있는 은행을 정해두고, 해당 은행에서 돈을 뽑을 시에 신분을 증명하게하는 방법이 있습니다.

Confidential Computing은 내 데이터가 내가 허용한 하드웨어에서 안전하게 데이터가 처리됨을 보장합니다.
이를 위해 안전하게 처리 될 수 있는 하드웨어(CCA)가 필요하고,
하드웨어가 안전한지 증명할 수 있는 방법(Attestation) 또한 제공해야 합니다.

여기서의 가정은 해당 은행을 신뢰하는 것 입니다.
은행원을 나쁜 뜻을 품고 통장에서 돈을 빼어 가려 했을 때, 그것도 방지 할 수 있을까요?

CCA는 커널과 같은 Privileged Software나 다른 Hardware Agents로부터로도 내 데이터를 보호 할 수 있는 환경을 제공합니다.

## Confidential compute: Realms
Arm CCA는 Confidential Compuring을 지원하기 위해 `Realm`과 `Dynamic TrustZone`을 추가하였습니다.

### Realm
발음하기 어려운 Realm은 왕국이라는 뜻을 가지고 있습니다. 나의 왕국에서만 나의 데이터가 사용되어야 함을 의미하는 것 같습니다.

Confidential Computing은 클라우드 환경에선 이미 널리 사용되고 있는 기술 입니다. 이를 위해 Intel은 SGX를 제공하죠.
SGX에선 Enclave라는 용어로 Realm에 상응하는 컴퓨팅 환경을 제공합니다.
Enclave는 고립된 영토라는 뜻을 가지고 있고, 개인적으론 Enclave 용어가 더 와닿습니다.

Realm은 증명가능한 고립된 컴퓨팅 환경이고 CCA는 이를 지원하기 위해 Realm Management Extension(RME)를 도입하였습니다.
데이터는 Realm에서 동작할 때만 복호화되고, 이 시점엔 다른 누구도 내 데이터를 볼 수 없습니다.

데이터는 메모리에 올라가니 메모리를 물리적으로 Realm만 접근 할 수 있게 합니다.
(물론 메모리에 올라와 있을 땐 암호화 되어있고, 실제 CPU에서 컴퓨팅시에만 복호화 됩니다.)
이를 위해 기존의 Normal World, Secure World에서 Realm World와 Root World가 추가 되었습니다.

### Dynamic TrustZone
Arm의 이전 세대의 보안기능인 TrustZone은 메모리를 애초에 Static하게 정해서 그 부분만 사용해야 하는 한계가 있었습니다.
메모리가 정해지고(Carved out)나면 TrustZone에서 동작하는 애플리케이션은 총 메모리보다 적은양만 사용해야합니다 :(

RME는 다음 세대 답게 Runtime에도 메모리를 Dynamic하게 할당 할 수 있게 합니다.
이를 위해 추가된 하드웨어로 모든 메모리 접근 시에 어떤 영역에서 접근하는지 Access Control을 합니다.
좀 더 기술 적인 표현으로는 메모리 접근 시 World 간 Isolation Boundaries를 확인 합니다.

## CCA Component
하드웨어 이외에도 많은 부분이 추가 되었습니다. TF-A는 FVP라는 하드웨어 에뮬레이터를 통해 아래의 기능들을 구현해놨습니다.
2022년 3월 기준으로 RME와 일부 RMM의 기능 테스트가 들어가 있으며, 메일링 리스트의 답변에 따르면 2022년 하반기에 RMM에 대한 SPEC이 TF-A에 적용 될 것으로 예상됩니다.

![CCA Component](https://developer.arm.com/-/media/Arm%20Developer%20Community/Images/Block%20Diagrams/Arm%20Confidential%20Compute%20Architecture/Components-of-ArmCCA.png?revision=cd595ff9-86eb-47c6-9719-c4d7e1fd40f5&h=582&w=943&la=en&hash=67108E9C793D281EBCFB997156C3BE1535831B80)

## Realm Management Extention (RME)
*[TF-A의 RME Section](https://trustedfirmware-a.readthedocs.io/en/latest/components/realm-management-extension.html#rme-support-in-tf-a)를 확인해보면 CCA의 실체에 한발 더 다가설 수 있습니다.*

RME는 Arm CCA의 하드웨어 컴포넌트 입니다.
TF-A v2.6이후로 RME를 지원하기 시작했습니다.
TF-A에 RME를 Enable시켜 빌드하고 실행해보겠습니다.

### RME Support in TF-A
아래 그림은 Arm CCA의 소프트웨어 아키텍처를 나타냅니다.
TF-A는 EL3의 Firmware입니다.
Root와 Realm을 위한, 두 개의 Security State와 Address가 추가 되었습니다.

TF-A는 Root Wrold에서 동작하는 Firmware입니다.
Realm World에선 RMM(Realm Management Monitor Firmware)가 동작합니다.

**RMM은 Realm VMs의 실행, Normal World의 Hypervisor와 Interaction을 관리합니다.**

![cca s/w arch](https://trustedfirmware-a.readthedocs.io/en/latest/_images/arm-cca-software-arch.png)

RME는 Arm CCA를 지원하기 위한 Hardware Extention입니다.
RME를 지원하기 위해서 TF-A에 다양한 변화가 들어왔습니다.

#### Changes to translation table library
RME는 Root와 Realm에 대한 PAS(Physical Address Space)를 추가 하였습니다.
이것을 지원하기 위해 `MT_ROOT`와 `MT_REALM`이 XLAT (Translation Tables) Library에 추가 되었습니다.
해당 매크로는 Root와 Realm에대해 Memory Region을 구성하는데 사용됩니다.

#### Boot Flow changes
전형적인 TF-A의 Boot Flow에서는, BL2가 Secure-EL1에서 동작합니다.

그러나 RME가 Enable 되면, TF-A는 Root World에서 EL3로 동작합니다.
그러므로 BL2는 EL3에서 동작하도록 변경 됩니다.

추가로 RMM은 BL2에 의해 Realm PAS에 로드됩니다.

1. BL1은 BL2를 로드하여 EL3에서 실행 시킵니다.
2. BL2는 RMM을 포함한 이미지를 로드합니다.
3. BL2는 BL31로 Control을 넘겨줍니다.
4. BL31은 SPM을 초기화 합니다
5. BL31은 RMM을 초기화 합니다.
6. BL31는 Normal World의 S/W로 Control을 넘깁니다.

#### Granule Protection Table(GPT) library
4개의 PAS에대한 Isolation은 GPC(Granule Protection Check)에 의해 강제 됩니다.
모든 Address Translation에 대해 MMU는 GPC를 수행합니다.
GPC는 Root Wrold에 있는 GPT를 사용합니다.
GPT는 모든 Page(Granule)에 대한 PAS를 Assign합니다.

GPT library는 GPTs를 초기화 하는 API와
서로 다른 PAS간 Granule을 Transition하는 API를 제공합니다.

#### RMM Dispatcher (RMMD)
RMMD는 Realm World로 swiching을 handle하는
새로운 Standard Runtime Service입니다.
**RMMD는 RMM을 초기화하고, RMI(Realm Management Interface)를 handle합니다.**
RMI는 Normal World와 Realm World로부터 발생되는 SMC Call을 의미합니다.

#### Test Realm Payload (TRP)
TRP는 R-EL2에서 실행되는 Test Payload와
EL3 firmware를 테스트하기 위해 RMI, R-EL2와 EL3의 인터페이스를 구현합니다.

### Building and running TF-A with RME
RME를 Enable하기 위해서는 ENABLE_RME build flag를 set해야합니다.
현재 FVP Platform에서만 지원합니다.

#### Building TF-A with TF-A Tests
TF-A Tests는 Normal World의 Payload입니다.(BL33)
TF-A Tests를 빌드하면 `tftf.bin`이 생성 됩니다.

TF-A를 빌드하면 `bl1.bin`과 `fip.bin`이 생성 됩니다.
TRP는 `fip.bin`에 포함됩니다.

#### Running the tests
생성된 바이너리는 FVP상에서 테스트를 수행 할 수 있습니다.

---

### Question
1. Armv9 아키텍처를 따르는 어떤 CPU에서 RME를 지원하나요?
2. RMM은 누구에의해, 어디로 로드 되나요?
3. RMM와 RMMD는 각각 어떤 이미지에 포함되어 있나요?
4. RMM은 누구에 의해 초기화 되나요?
5. RME를 지원하면서 TF-A 변화된 3가지는 무엇인가요?
