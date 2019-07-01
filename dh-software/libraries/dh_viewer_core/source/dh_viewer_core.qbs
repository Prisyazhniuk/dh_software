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
        "fft_processing_statistics.h",
        "fft_processing_statistics_model.cpp",
        "fft_processing_statistics_model.h",
        "fft_processor.cpp",
        "fft_processor.h",
    ]   
}
