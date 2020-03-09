# Software Guard Extensions

운영체제는 권한 모델을 사용하여
자신만의 메모리 영역을 보호한다.  

SGX는 Skylake 이후
인텔 CPU칩에 추가된 보안 관련 명령어 셋으로,
이를 활용하면
유저 모드에서 동작하는
프로그램도
자신만 볼 수 있는
고유한 메모리 영역을
정의 할 수 있다.  

SGX가 보호하는
프로그램 내 메모리 영역을
엔클레이브(Enclave)라 한다.
엔클레이브에 저장된 데이터는
프로그램 자신만 볼 수 있다.
운영체제나 루트(root) 권한이 있는
프로세스도 볼 수 없다.

SGX의 위협 모델(Threat model)은
"엔클레이브를 제외한
모든 프로세스는
안전하지 않다."이다.
모든 프로세스에는
운영체제와 하이퍼바이저(hypervisor)도
포함 된다. SGX는 암호화를 사용하여
이 위협 모델로부터
프로그램을 보호한다.
엔클레이브는 CPU 내에서
동작할 때를 제외하고
항상 암호화 된 채로
존재한다.
따라서 외부에서는
엔클레이브의 암호화된 형태 밖에
볼 수 없다.
SGX는 부채널 공격(side channel attack)에 대한
보안성은 보장하지 않기 때문에 
프로그램은 자체적으로 부채널 공격에
안전하게 설계 되어야 한다.

SGX는 클라우드 컴퓨팅,
DRM(Digital Rights Management)와 같이
외부로부터 보호되어야 데이터가
있을 때 유용하도록 설계 되었다.


# References
- [Software Guard Extensions](https://en.wikipedia.org/wiki/Software_Guard_Extensions)
