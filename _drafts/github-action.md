What are GitHub Actions?
GitHub Actions are a flexible way to automate nearly e
very aspect of your team's software workflow. Here are just a few of the ways teams are using GitHub Actions:

Automated testing (CI)
Continuous delivery and deployment
Responding to workflow triggers using 
issues, 
@ mentions, labels, and more
Triggering code reviews
Managing branches
Triaging issues and pull requests

## Github Action
소프트웨어 워크-플로우(Workflow)를 자동화 시켜주는 도구이다.
워크-플로우는 레파지토리에 코드로 작성되며, 재사용, 공유가 가능하다.
- 자동화 테스트 (CI)
- 지속적 배포 (CD)
- 이벤트들(push, pull request, ...)에 대한 훅(hook)
-

jobs: is the base component of a workflow run
build: is the identifier we're attaching to this job
name: is the name of the job, this is displayed on GitHub when the workflow is running
runs-on: defines the type of machine to run the job on. The machine can be either a GitHub-hosted runner or a self-hosted runner.
steps: the linear sequence of operations that make up a job
uses: actions/checkout@v1 uses a community action called checkout to allow the workflow to access the contents of the repository
uses: ./action-a provides the relative path to the action we created in the action-a directory of the repository
with: is used to specify the input variables that will be available to your action in the runtime environment. In this case, the input variable is MY_NAME, and it is currently initialized to "Mona".


## Components
- Workflow: job으로 구성되는 하나의 워크-플로우, 이벤트에 의해 트리거
- Event: Workflow를 트리거 시키기 위한 이벤트 (push, pull request)
- Job:
    - 여러 Step의 뭉치
    - 가상환경에서 실제 실행되는 단위 (runs-on: ubuntu-latest)
    - 환경 변수를 Input으로 넣을 수 있음 (with: MY_NAME: "Mona")
- Step: Job을 구성하는 실행 단위
    - Workflow가 repository에 접근 할 수 있도록 함. (uses: actions/checkout@v1)
    - 액션을 실행 시킬 수 있음 (uses: ./action-a)

```
name: A workflow for my Hello World file
on: push

jobs:
build:
name: Hello world action
runs-on: ubuntu-latest
steps:
- uses: actions/checkout@v1 # workflow to access the contents of the repository
- uses: ./action-a
with:
MY_NAME: "Mona"
```

https://zzsza.github.io/development/2020/06/06/github-action/
https://lab.github.com/githubtraining/github-actions:-hello-world
