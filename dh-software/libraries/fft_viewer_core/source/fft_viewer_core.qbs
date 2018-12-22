import qbs

DhCppStaticLibrary {
    name: "fft_viewer_core"

    Depends { name: "image_processing" }

    Export {
        Depends { name: "image_processing" }
    }

    files: [
        "fft_viewer_core.cpp",
        "fft_viewer_core.h",
    ]
}
