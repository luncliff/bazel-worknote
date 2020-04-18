#
# References
#   https://docs.bazel.build/versions/2.2.0/windows.html#using-bazel-on-windows-1
#   https://docs.bazel.build/versions/2.2.0/windows.html#build-c-with-clang
#
platform(
    name = "x64-windows-clang",
    constraint_values = [
        "@platforms//cpu:x86_64",
        "@platforms//os:windows",
        "@bazel_tools//tools/cpp:clang-cl",
    ],
)
