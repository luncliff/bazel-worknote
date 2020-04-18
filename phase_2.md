# 2회차

프로그램을 만들고 실행할 수 있게 되었으니, 외부 라이브러리를 가져다 쓰는 방법을 알아야겠다.

* [Working with external dependencies](https://docs.bazel.build/versions/2.2.0/external.html)

## Setup

문서를 읽어보니 .bazel 확장자를 대신 사용할 수 있는 것 같다.
BUILD는 CMake 프로젝트들이 자주 사용하는 이름이므로, .bazel 확장자를 사용하도록 한다. 일관성을 위해 WORKSPACE도 이를 따른다.

```
.
├── BUILD.bazel
├── WORKSPACE.bazel
├── externals
│   ├── BUILD.bazel
│   ├── zlib.bazel
│   └── curl.bazel
├── ...
└── test
    ├── BUILD.bazel
    ├── test_1.cpp
    └── test_2.cpp
```

### Externals

여기서는 cURL 라이브러리(이하 libcurl)를 사용한다.
세부적인 내용은 https://curl.haxx.se/libcurl/ 에서 검색해가면서 ... 

설명을 보니 URL로 다운로드하여 외부 의존성을 준비하는 것이 가능해 보인다.
의존성을 관리할때는 신경써야 할 것들이 많이 있지만, 여기서는 단순히 의존성을 전파하는 형태로 사용한다고만 전제한다.

다행스럽게도 이를 미리 작성한 선구자(라고 쓰고 먼저 삽질한 사람이라고 한다)가 있었다. Thank you, [vnvo2409](https://github.com/vnvo2409)!

* https://github.com/vnvo2409/curl_bazel  
  라이선스 명시가 없었기 때문에, 여기서는 파일들을 거의 그대로 재사용하였다.

```bazel
# WORKSPACE.bazel
workspace(name = "wcs")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "curl",
    build_file = "//externals:curl.bazel",
    sha256 = "e9c37986337743f37fd14fe8737f246e97aec94b39d1b71e8a5973f72a9fc4f5",
    strip_prefix = "curl-7.60.0",
    urls = [
        "https://curl.haxx.se/download/curl-7.60.0.tar.gz",
        "https://mirror.bazel.build/curl.haxx.se/download/curl-7.60.0.tar.gz",
    ],
)

# ...
```

`sha256` 필드에는 대문자를 사용할수가 없었다. (귀찮게...)

```ps1
$fname = "curl-7.60.0.tar.gz"
Invoke-WebRequest "https://curl.haxx.se/download/$fname" -OutFile $fname
$hash = Get-FileHash $fname -Algorithm SHA256
Write-Output $hash.HASH.ToLower()
```

이 경우는 소스코드를 다운로드 받았으므로, 사용하기에 앞서 빌드/설치를 거쳐야 한다.
Bazel을 사용하지 않는 프로젝트라면, 이를 사용할 수 있도록 .bazel파일을 작성해줘야 한다는 것인데...
이를 `build_file`로 명시하는 것 같다. [externals/curl.bazel](./externals/curl.bazel)에서 처리하도록 하였다.

```bazel
# externals/curl.bazel
cc_library(
    name = "curl",
    # ...
    deps = [
        "@boringssl//:ssl",
        "@zlib",
    ],
)
```

[`deps`를 보니, `cc_library`에서 명시한 `name`을 사용하되, 앞에 `@`를 붙이는 것으로 보인다](https://docs.bazel.build/versions/2.2.0/be/c-cpp.html). 이를 [Label](https://docs.bazel.build/versions/2.2.0/build-ref.html#labels)이라고 한다고...

https://github.com/google/boringssl 는 Bazel을 지원한다는 점에서 vnvo2409의 예제는 아주 의도적임을 알 수 있다.

```bazel
# test/BUILD.bazel
# ...

cc_test(
    name = "test_2",
    srcs = [
        "test_2.cpp"
    ],
    copts = [
        "/W4", "/std:c++14",
    ],
    deps = [
        "@curl//:curl",
    ],
)
```

특별히 libcurl을 사용하기 위한 경로 / 빌드 옵션 설정이 없는 것에 주의.

### Project

### Source Code

[test_2.cpp](./test/test_2.cpp)는 CURL 예제 certinfo를 사용하였다. 

* https://curl.haxx.se/libcurl/c/certinfo.html

## Usage

### Build/Test

실험을 몇번 해보니 externals 폴더 역시 Bazel Workspace로 인식되어야 하는 모양이다.
내용이 없는 BUILD.bazel 파일을 만들어주는 것으로 대응하였다.

한글 환경에서는 C4819(codepage) 경고가 너무 많이 나와서 빌드할 때 이를 막도록 하였다.

```
bazel build //test:test_2 --cxxopt /utf-8
```

비슷한 방법으로 컴파일러를 지정할 수 있다는 것을 알게 되었다.

```
bazel test //test:test_2 --compiler=clang-cl --verbose_failures
```

Bazel 테스트에서는 출력 결과가 보이지 않으므로, WORKSPACE.bazel 옆에 생성된 bazel-bin 폴더에서 프로그램을 찾아 직접 실행시켜 보았다.

> bazel-testlogs 폴더에 `cc_test`마다 .log 파일이 남는다.

```
./bazel-bin/test/test_2.exe
1 certs!
Subject:C=US, ST=California, L=Los Angeles, O=Internet Corporation for Assigned Names and Numbers, OU=Technology, 
CN=www.example.org
Issuer:C=US, O=DigiCert Inc, CN=DigiCert SHA2 Secure Server CA
Version:2
...
```

