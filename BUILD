# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
    name = "xlayout-helper",
    srcs = glob(["src/*.cxx", "src/*.h"]),
    copts = ["-std=c++17"],
    linkopts = ["-lX11"]
)