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
        "blob_detection_statistics.h",
        "blob_detection_statistics_model.cpp",
        "blob_detection_statistics_model.h",
        "blob_detector.cpp",
        "blob_detector.h",
        "error_reporter.cpp",
        "error_reporter.h",
        "hologram_processor.cpp",
        "hologram_processor.h",
        "processing_settings.h",
        "processing_settings_model.cpp",
        "processing_settings_model.h",
        "processing_statistics.h",
        "processing_statistics_model.cpp",
        "processing_statistics_model.h",
    ]   
}
