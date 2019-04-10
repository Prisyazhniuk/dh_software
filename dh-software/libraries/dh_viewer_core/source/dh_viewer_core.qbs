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
        "blob_detector.cpp",
        "blob_detector.h",
        "fft_processing_statistics.h",
        "fft_processor.cpp",
        "fft_processor.h",
        "processing_statisctics_model.cpp",
        "processing_statisctics_model.h",
    ]   
}
