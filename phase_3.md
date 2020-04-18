# 3회차

이제 모듈화와 관련하여 몇가지를 확인할 필요가 있다.  

앞서 [2회차](./phase_2.md)에서 Bazel에서 약 400여개의 파일을 빌드하는 것을 확인할 수 있었는데. 이들이 어딘가에 **설치**되었다는 내용은 없었다.
그렇다면 모듈을 정의했을 때 빌드의 결과물은 어디에 생성되며, 어떤 형태를 가지는가?

#### 모듈에 대한 정의

개발자들에게 모듈(module)의 정의는 제각기 다를 수 있으므로, 이 저장소에서는 **소스 코드와 이들을 위한 빌드 설정이 관리되는 단위**라고 정의하겠다.

여기서 신경써야 할 부분은 빌드 설정인데, 보통 툴에서는 3가지를 지정할 수 있어야 한다.
이는 소스코드(Text)로부터 프로그램(Program Image)을 만들어내기까지 크게 3단계 과정을 거치기 때문이다.

1. `#include ...`를 위한 경로 (Text -> Translation Unit)
2. 컴파일러 옵션 (Translation Unit -> Object)
3. 링커 옵션 (Object -> Program Image)

## Setup

최초 Bazel 빌드를 시작하면 ${workspace} 폴더를 기준으로 몇가지 폴더가 생성되는데, 각각을 확인해보았다.

* bazel-bin/: object, lib, exe
    * external/
    * test/
* bazel-contents/external/: archive(소스코드)
* bazel-testlogs/: test log

### Project

### Source Code

>
> TBA  
> * 라이브러리(즉, 모듈) header들의 include 형태?
>

## Usage

> TBA

### Build

> TBA

### Test

> TBA
