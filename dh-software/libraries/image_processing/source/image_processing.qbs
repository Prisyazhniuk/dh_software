import qbs

DhCppStaticLibrary {
    name: "image_processing"

    Depends { name: "common" }

    Export {
        Depends { name: "common" }
    }

    additionalStaticLibraries: [
        "opencv_world451",
        "ippcore",
        "ippi",
        "ipps",
        "ippvm",
        "ippcv",
    ]

    files: [
        "dft.cpp",
        "dft.h",
        "gabor.cpp",
        "gabor.h",
        "image_32fc.cpp",
        "image_32fc.h",
        "image_converter.cpp",
        "image_converter.h",
        "image_processing.cpp",
        "image_processing.h",
        "spectrum_shifter.cpp",
        "spectrum_shifter.h",
    ]

    Group {
        name: "dll"
        qbs.install: true
        qbs.installDir: "bin"
        files: [
            project.opencv_bin_path.concat("/libopencv_world451.dll"),
            project.ipp_bin_path.concat("/ippcore.dll"),
            project.ipp_bin_path.concat("/ippi*.dll"),
            project.ipp_bin_path.concat("/ipps*.dll"),
            project.ipp_bin_path.concat("/ippvm*.dll"),
            project.ipp_bin_path.concat("/ippcv*.dll"),
        ]
    }
}
