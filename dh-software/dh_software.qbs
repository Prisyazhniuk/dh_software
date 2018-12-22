import qbs
import qbs.Process

Project {
    name: "dh-software"
    qbsSearchPaths: "qbs"
    minimumQbsVersion: "1.12.2"

    references: [
        "applications/fft_viewer/fft_viewer.qbs",

        "libraries/fft_viewer_core/source/fft_viewer_core.qbs",
        //"libraries/fft_viewer_core/tests/fft_viewer_core_tests.qbs",

        "libraries/image_processing/source/image_processing.qbs",
        //"libraries/image_processing/tests/image_processing_tests.qbs",
    ]

    property pathList additionalIncludePaths: []

    property pathList additionalLibraryPaths: []

    property stringList additionalCXXFlags: {
        flags = [ "-Wno-unused-parameter", "-D_USE_MATH_DEFINES" ]

        return flags
    }

    AutotestRunner {}
}
