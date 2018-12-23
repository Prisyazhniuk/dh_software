import qbs

DhCppStaticLibrary {
    files: [
        "dh_thread.cpp",
        "dh_thread.h",
        "exceptions/argument_exception.h",
        "exceptions/dh_exception.cpp",
        "exceptions/dh_exception.h",
        "exceptions/exception_source.cpp",
        "exceptions/exception_source.h",
        "type_info_helpers.cpp",
        "type_info_helpers.h",
    ]
    name: "common"
}
