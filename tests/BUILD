cc_test(
    name = "test_obj_trimesh",
    srcs = ["test_obj_trimesh.cpp"],
    copts = ["-Ilib"],
    data = ["//tests:data/test.obj"],
    deps = [
        "//lib:trimesh",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "test_proximity_check",
    srcs = ["test_proximity_check.cpp"],
    copts = [
        "-Ilib",
        "-Iext/eigen-3.4.0",
    ],
    deps = [
        "//ext:eigen",
        "//lib/simulation",
        "@com_google_googletest//:gtest_main",
    ],
)

