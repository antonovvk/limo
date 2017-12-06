cc_binary(
    name = "limo",
    srcs = [
        "llinear.h",
        "llinear.cpp",
        "model.h",
        "main.cpp",
    ],
    deps = [
        "@clar//:lib",
        "@liblinear//:liblinear",
    ],
    copts = [
        "-std=c++11",
        "-Wall",
    ],
    linkopts = [
        "-lm",
    ],
    visibility = ["//visibility:public"],
)
