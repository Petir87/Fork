# TensorFlow API backwards compatibility tests.

load(
    "//tensorflow:tensorflow.bzl",
    "py_test",
    "tf_cc_binary",
)

package(
    # copybara:uncomment default_applicable_licenses = ["//tensorflow:license"],
    default_visibility = ["//tensorflow/tools/api:__subpackages__"],
    licenses = ["notice"],
)

exports_files([
    "README.txt",
    "API_UPDATE_WARNING.txt",
])

py_test(
    name = "api_compatibility_test",
    srcs = ["api_compatibility_test.py"],
    data = [
        "//tensorflow/tools/api/golden:api_golden_v1",
        "//tensorflow/tools/api/golden:api_golden_v2",
        "//tensorflow/tools/api/tests:API_UPDATE_WARNING.txt",
        "//tensorflow/tools/api/tests:README.txt",
        "//third_party/py/numpy/tf_numpy_api:api_golden",
    ],
    python_version = "PY3",
    srcs_version = "PY3",
    tags = [
        "no_mac",  # b/198669105
        "no_pip",
        "no_rocm",
        "no_windows",  # Bugs due to some paths.
    ],
    deps = [
        "@six_archive//:six",
        # copybara:uncomment "//third_party/py/tensorflow:tensorflow_compat_v1_estimator",
        # copybara:uncomment "//third_party/py/tensorflow:tensorflow_compat_v2_estimator",
        "//tensorflow:tensorflow_py",
        "//tensorflow/python/lib/io:lib",
        "//tensorflow/python/platform:client_testlib",
        "//tensorflow/python/platform:resource_loader",
        "//tensorflow/python/platform:tf_logging",
        "//tensorflow/tools/api/lib:python_object_to_proto_visitor",
        "//tensorflow/tools/common:public_api",
        "//tensorflow/tools/common:traverse",
    ],
)

py_test(
    name = "module_test",
    srcs = ["module_test.py"],
    python_version = "PY3",
    srcs_version = "PY3",
    tags = [
        "no_windows",  # Failing due to missing API symbols.
    ],
    deps = [
        "//tensorflow:tensorflow_py",
        "//tensorflow/python/platform:client_testlib",
    ],
)

tf_cc_binary(
    name = "convert_from_multiline",
    srcs = ["convert_from_multiline.cc"],
    deps = [
        "//tensorflow/core:lib",
        "//tensorflow/core:op_gen_lib",
    ],
)
