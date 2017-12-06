cc_binary(
    name = "limo",
    srcs = [
        "model.h",
        "llinear.h",
        "llinear.cpp",
        "pwgmd.h",
        "pwgmd.cpp",
        "main.cpp",
    ],
    deps = [
        "@clar//:lib",
        "@decmod//:decmod",
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
