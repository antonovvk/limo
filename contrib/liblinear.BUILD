cc_library(
    name = "blas",
    hdrs = [
        "blas/blas.h",
        "blas/blasp.h",
    ],
    srcs = [
        "blas/daxpy.c",
        "blas/ddot.c",
        "blas/dnrm2.c",
        "blas/dscal.c",
    ],
    copts = [
        "-Wall",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "liblinear",
    hdrs = [
        "linear.h",
        "tron.h",
    ],
    srcs = [
        "linear.cpp",
        "tron.cpp",
    ],
    deps = [
        ":blas",
    ],
    copts = [
        "-std=c++11",
        "-Wall",
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "predict",
    srcs = [
        "predict.c",
    ],
    deps = [
        ":liblinear",
    ],
    copts = [
        "-Wall",
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "train",
    srcs = [
        "train.c",
    ],
    deps = [
        ":liblinear",
    ],
    copts = [
        "-Wall",
    ],
    visibility = ["//visibility:public"],
)
