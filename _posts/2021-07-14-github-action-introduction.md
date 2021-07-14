---
layout: post
title: Introduction to GitHub Actions
tags: [github]
---

## [Github Action Introduction](https://docs.github.com/en/actions/learn-github-actions/introduction-to-github-actions)
> What are GitHub Actions?
> GitHub Actions makes
> it easy to automate
> all your software workflows.

Github Action은
Github에서 제공하는 CI/CD 도구이다.
CI/CD가 적용된 프로젝트는
코드가 레파지토리에 반영되면,
**자동으로 테스트를 거쳐 배포**한다.

추상화 시켜보면, 레파지토리에 자신의 `이벤트 핸들러`를 등록하는 것이다.
```
코드가 반영되면, 자동화 테스트 ... 배포한다.
-----------  -------------------
   event      action (handler)
```

## [Overview](https://docs.github.com/en/actions/learn-github-actions/introduction-to-github-actions#overview)
Github Action은 어떻게 자동적으로 테스트를 실행 할 수 있을까?
![diagram](https://docs.github.com/assets/images/help/images/overview-actions-simple.png)

이벤트가 발생하면, 워크-플로우(Workflow)를 트리거한다.
워크-플로우는 잡(Job)을 가지고 있고, 잡은 여러 스텝으로 구성 된다.
각 스텝에서 테스트와 같은 액션(Action)을 실행 시킬 수 있다.

```
Event -> Workflow -> Job -> Step -> Action
```

## [Component](https://docs.github.com/en/actions/learn-github-actions/introduction-to-github-actions#the-components-of-github-actions)
- 워크-플로우
	- 자동화를 위해 레파지토리에 추가 할 수 있는 절차다.
	- 이벤트에 의해 트리거 되는 하나 이상의 잡들로 구성된다.
	- 빌드, 테스트, 패키징, 릴리즈, 배포를 위해 사용 할 수 있다.
- 이벤트
	- 워크-플로우를 트리거 하는 이벤트이다.
	- 레파지토리에 커밋이 푸쉬 되거나, 풀-리퀘스트가 생성 됐을 때를 지정 할 수 있다.
- 잡
	- 같은 러너(실행환경)에서 실행되는 스텝들의 집합이다.
	- 하나의 워크플로우에 여러개의 잡이 있을 때, 각 러너는 병렬적으로 실행 된다.
	- 빌드 -> 테스트 와 같이 잡 간에 디펜던시를 주어, 순차적으로 실행 시킬 수 있다.
- 액션
	- 액션은 여러 명령어들을 하나의 스텝에서 실행 시킬 수 있게 묶어 놓은 것이다.
		- 명령어들을 쉘 스크립트로 만들어, 하나의 쉘 스크립트만 실행 시켜도 되는 원리
	- 액션은 워크-플로우에서 재사용 가능한 모듈(or building block)이다.
	- 자신의 워크-플로우에, [미리 정의된 액션들을](https://github.com/actions) 가져와 실행 시킬 수 있다.
- 러너
	- 러너는 깃헙에서 호스트하는 서버이다.
		- 자신의 서버를 사용 할 수도 있다.
	- 워크플로우, 잡이 동작하는 환경이다.
	- 하나의 러너는 하나의 잡을 실행 시키며, 진행사항, 로그, 결과를 깃헙에 보여준다.
	- Ubuntu Linux, Microsoft Windows, and macOS를 지원한다.

## Example
![example](https://docs.github.com/assets/images/help/images/overview-actions-event.png)
```yaml
name: learn-github-actions # 워크-플로우의 이름
on: [push] # 이벤트 (push가 되었을 때 잡들을 실행)
jobs:
  check-bats-version: # Job 이름
    runs-on: ubuntu-latest # 러너; 우분투에서 실행
    steps: # Job을 구성하는 스텝들, 아래부터 순차적으로 실행
      - uses: actions/checkout@v2 # * 깃헙 액션이, 레파지토리에 접근 할 수 있도록 허용 *
      - uses: actions/setup-node@v2 # 노드를 설치하는 미리 정의 된 액션 실행
        with:
          node-version: '14' # 노드 액션에 파라미터 제공
      - run: npm install -g bats # 노드 환경이 갖춰져서 bats 실행
      - run: bats -v # bats 버전 확인
```