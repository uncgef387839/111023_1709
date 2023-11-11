package(default_visibility = [
    "//lib:__subpackages__",
    "//src:__pkg__",
    "//tests:__pkg__",
])

cc_library(
    name = "vk_display",
    srcs = ["vk_display.cpp"],
    hdrs = ["vk_display.h"],
    copts = [
        "-lglfw",
        "-lvulkan",
        "-ldl",
        "-lpthread",
        "-lX11",
        "-lXxf86vm",
        "-lXrandr",
        "-lXi",
        "-Iext/eigen-3.4.0/",
    ],
    data = [
        "//lib/display/shaders:frag",
        "//lib/display/shaders:vert",
    ],
    deps = [
        "deligate",
        "display",
        "window",
    ],
)

cc_library(
    name = "display",
    hdrs = ["display.h"],
)

cc_library(
    name = "deligate",
    srcs = ["deligate.cpp"],
    hdrs = ["deligate.h"],
    copts = [
        "-Iext/eigen-3.4.0/",
    ],
    deps = [
        "//lib:object",
    ],
)

cc_library(
    name = "window",
    srcs = ["window.cpp"],
    hdrs = ["window.h"],
)
