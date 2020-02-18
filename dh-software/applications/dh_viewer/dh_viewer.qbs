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
        "draggable_cursor.cpp",
        "draggable_cursor.h",
        "float_item_delegate.cpp",
        "float_item_delegate.h",
        "main.cpp",
        "main_window.cpp",
        "main_window.h",
        "main_window.ui",
    ]   
}
