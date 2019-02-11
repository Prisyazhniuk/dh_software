import qbs

DhCppStaticLibrary {
    name: "qt_helpers"

    Depends {
        name: "Qt";
        submodules: [ "core", "gui", "widgets", "opengl" ]
    }

    Export {
        Depends {
            name: "Qt";
            submodules: [ "core", "gui", "widgets", "opengl" ]
        }
    }

    files: [
        "graphics_view.cpp",
        "graphics_view.h",
        "graphics_view_wheel_scroll.cpp",
        "graphics_view_wheel_scroll.h",
        "icons.qrc",
    ]   
}
