# 1회차

우선 기본적인 테스트 프로그램을 만드는 방법을 알고 싶다.
이를 위해 문서 2개를 정독했다

* [Introduction to Bazel: Common C++ Build Use Cases](https://docs.bazel.build/versions/2.2.0/cpp-use-cases.html)
* [C++ and Bazel](https://docs.bazel.build/versions/2.2.0/bazel-and-cpp.html)


추가로 읽는 중인 문서

* [A User's Guide to Bazel](https://docs.bazel.build/versions/2.2.0/guide.html)

## Setup

파일 구성은 다음과 같이 맞추었다.

```
.
├── BUILD
├── WORKSPACE
├── readme.md
└── test
    ├── BUILD
    └── test_1.cpp
```

### Project

기본적으로 배포된 .zip 파일들을 사용하는 방법으로 외부 모듈들을 확보할 계획이므로, `WORKSPACE`는 간단하게 이정도만. `wcs`는 Web Contents Support의 약자(acronym).

```bazel
# WORKSPACE
workspace(name = "wcs")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
```

프로젝트는 x64, Windows 환경을 기본으로 한다. 이는 최상위 `BUILD` 파일에 작성.  
~~[뿌슝빠슝 `clang-cl`을 쓰는 윈도우 개발자가 있다?](https://chocolatey.org/packages/llvm)~~

```bazel
# BUILD
platform(
    name = "x64-windows-clang",
    constraint_values = [
        "@platforms//cpu:x86_64",
        "@platforms//os:windows",
        "@bazel_tools//tools/cpp:clang-cl",
    ],
)
```

테스트 폴더에도 `BUILD`를 작성해서 어떤 일을 할지 알려줘야 한다.

```bazel
# test/BUILD
cc_test(
    name = "test_1",
    srcs = ["test_1.cpp"],
    copts = ["/W4", "/std:c++latest"],
)
```

### Build/Run

[test_1.cpp](./test/test_1.cpp)는 간단히 [wmain](https://docs.microsoft.com/en-us/cpp/c-language/using-wmain)의 인자를 출력한다.

CLI에서 빌드를 명령할 때, 표기 순서는 `//folder/name`.

```
bazel build //test:test_1
```

`cc_test`는 아래처럼 실행할 수 있다. 테스트 프로그램에 인자를 줄 수 는 없는 모양.

```
bazel test //test:test_1 
```

빌드 혹은 테스트가 실패한 경우, log 파일의 경로를 보여준다.
bazel- 폴더들을 생성하는 것은 그대로 두고는 왠지 사용자의 홈 디렉토리에 빌드 폴더를 만들어 로그를 저장한다.
