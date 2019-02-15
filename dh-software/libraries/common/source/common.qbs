import qbs

DhCppStaticLibrary {
    name: "common"

    files: [
        "dh_math.cpp",
        "dh_math.h",
        "dh_string.cpp",
        "dh_string.h",
        "dh_thread.cpp",
        "dh_thread.h",
        "dh_timer.cpp",
        "dh_timer.h",
        "exceptions/argument_exception.h",
        "exceptions/dh_exception.cpp",
        "exceptions/dh_exception.h",
        "exceptions/exception_source.cpp",
        "exceptions/exception_source.h",
        "exceptions/image_processing_exception.h",
        "type_info_helpers.cpp",
        "type_info_helpers.h",
    ]    
}
