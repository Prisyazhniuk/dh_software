import qbs

DhCppStaticLibrary {
    name: "dh_viewer_core"

    Depends { name: "image_processing" }
    Depends { name: "common" }

    Depends {
        name: "Qt";
        submodules: [ "core", "gui", "widgets" ]
    }

    Export {
        Depends { name: "image_processing" }
        Depends { name: "common" }
    }

    files: [
        "draggable_cursor.cpp",
        "draggable_cursor.h",
        "error_reporter.cpp",
        "error_reporter.h",
        "hologram_processor.cpp",
        "hologram_processor.h",
        "image_loader.cpp",
        "image_loader.h",
        "intensity_graph_model.cpp",
        "intensity_graph_model.h",
        "processing_settings.h",
        "processing_settings_model.cpp",
        "processing_settings_model.h",
        "processing_statistics.h",
        "processing_statistics_model.cpp",
        "processing_statistics_model.h",
    ]   
}
