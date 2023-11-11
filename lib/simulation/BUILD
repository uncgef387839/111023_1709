package(default_visibility = [
    "//lib:__subpackages__",
    "//src:__pkg__",
    "//tests:__pkg__",
])

cc_library(
    name = "simulation",
    srcs = ["simulation.cpp"],
    hdrs = ["simulation.h"],
    copts = ["-Iext/eigen-3.4.0/"],
    deps = [
        "//lib:object",
    ],
)
