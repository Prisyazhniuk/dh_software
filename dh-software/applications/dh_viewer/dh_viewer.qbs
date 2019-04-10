import qbs

DhCppApplication {
    name: "dh_viewer"

    Depends { name: "dh_viewer_core" }
    Depends { name: "qt_helpers" }

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
