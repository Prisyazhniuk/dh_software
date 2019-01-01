import qbs

DhCppApplication {
    name: "fft_viewer"

    Depends { name: "fft_viewer_core" }

    Depends {
        name: "Qt";
        submodules: [ "core", "gui", "widgets" ]
    }

    files: [
        "main.cpp",
        "main_window.cpp",
        "main_window.h",
        "main_window.ui",
    ]   
}
