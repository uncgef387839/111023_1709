cc_binary(
    name = "run_sim_vk",
    srcs = ["run_sim_vk.cpp"],
    copts = [
        "-Iext/eigen-3.4.0/",
        "-g",
        "-Wall",
    ],
    data = [
        "//data:floor.obj",
        "//data:square.obj",
    ],
    linkopts = [
        "-lglfw",
        "-lvulkan",
    ],
    deps = [
        "//lib:loop",
        "//lib:trimesh",
        "//lib/display:vk_display",
        "//lib/display:window",
        "//lib/simulation",
    ],
)
