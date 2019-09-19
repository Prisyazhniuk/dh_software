import qbs

DhCppStaticLibrary {
    name: "dh_viewer_core"

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
        "error_reporter.cpp",
        "error_reporter.h",
        "hologram_processor.cpp",
        "hologram_processor.h",
        "image_loader.cpp",
        "image_loader.h",
        "processing_settings.h",
        "processing_settings_model.cpp",
        "processing_settings_model.h",
        "processing_statistics.h",
        "processing_statistics_model.cpp",
        "processing_statistics_model.h",
    ]   
}
