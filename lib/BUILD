package(default_visibility = [
    "//lib:__subpackages__",
    "//src:__pkg__",
    "//tests:__pkg__",
])

cc_library(
    name = "trimesh",
    srcs = ["trimesh.cpp"],
    hdrs = ["trimesh.h"],
)

cc_library(
    name = "utils",
    hdrs = ["utils.h"],
    copts = ["-Iext/eigen-3.4.0/"],
    deps = ["//ext:eigen"],
)

cc_library(
    name = "loop",
    hdrs = ["loop.h"],
    deps = [
        "//lib/display",
        "//lib/simulation",
    ],
)

cc_library(
    name = "object",
    srcs = ["object.cpp"],
    hdrs = ["object.h"],
    copts = ["-Iext/eigen-3.4.0/"],
    deps = [
        "//ext:eigen",
        "//lib:trimesh",
    ],
)
