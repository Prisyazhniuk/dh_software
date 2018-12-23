import qbs

DhCppStaticLibrary {
    name: "fft_viewer_core"

    Depends { name: "image_processing" }
    Depends { name: "common" }

    Export {
        Depends { name: "image_processing" }
        Depends { name: "common" }
    }

    files: [
        "fft_viewer_core.cpp",
        "fft_viewer_core.h",
    ]
}
