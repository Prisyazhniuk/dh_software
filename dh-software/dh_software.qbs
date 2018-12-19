import qbs
import qbs.Process

Project {
    name: "dh-software"
    qbsSearchPaths: "qbs"
    minimumQbsVersion: "1.12.2"

    references: [
        "applications/fft_viewer/fft_viewer.qbs"
    ]

    property pathList additionalIncludePaths: []

    property pathList additionalLibraryPaths: []

    property stringList additionalCXXFlags: {
        flags = [ "-Wno-unused-parameter", "-D_USE_MATH_DEFINES" ]

        return flags
    }

    AutotestRunner {}
}
