import qbs

DhCppStaticLibrary {
    name: "image_processing"

    Depends { name: "common" }

    Export {
        Depends { name: "common" }
    }

    additionalStaticLibraries: [
        "opencv_world400",
        "ippcore",
        "ippi",
        "ipps",
        "ippvm",
        "ippcv",
    ]

    files: [
        "binarizer.cpp",
        "binarizer.h",
        "blob.h",
        "blob_calculator.cpp",
        "blob_calculator.h",
        "dft.cpp",
        "dft.h",
        "gauss_filter.cpp",
        "gauss_filter.h",
        "image_32fc.cpp",
        "image_32fc.h",
        "image_converter.cpp",
        "image_converter.h",
        "image_processing.cpp",
        "image_processing.h",
        "label_marker.cpp",
        "label_marker.h",
        "spectrum_shifter.cpp",
        "spectrum_shifter.h",
    ]

    Group {
        name: "dll"
        qbs.install: true
        qbs.installDir: "bin"
        files: [
            project.opencv_bin_path.concat("/libopencv_world400.dll"),
            project.ipp_bin_path.concat("/ippcore.dll"),
            project.ipp_bin_path.concat("/ippi*.dll"),
            project.ipp_bin_path.concat("/ipps*.dll"),
            project.ipp_bin_path.concat("/ippvm*.dll"),
            project.ipp_bin_path.concat("/ippcv*.dll"),
        ]
    }
}
