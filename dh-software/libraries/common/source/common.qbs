import qbs

DhCppStaticLibrary {
    files: [
        "dh_exception.cpp",
        "dh_exception.h",
        "dh_thread.cpp",
        "dh_thread.h",
        "exception_source.cpp",
        "exception_source.h",
        "type_info_helpers.cpp",
        "type_info_helpers.h",
    ]
    name: "common"
}
