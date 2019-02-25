import qbs

DhCppStaticLibrary {
    name: "fft_viewer_core"

    Depends { name: "image_processing" }
    Depends { name: "common" }

    Depends {
        name: "Qt";
        submodules: [ "core", "gui" ]
    }

    Export {
        Depends { name: "image_processing" }
        Depends { name: "common" }
    }

    files: [
        "fft_viewer_core.cpp",
        "fft_viewer_core.h",
        "processing_statistics.h",
    ]   
}
