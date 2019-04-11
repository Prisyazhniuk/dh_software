import qbs

DhUnitTests {
    name: "image_processing_tests"

    Depends { name: "image_processing" }

    files: [
        "binarizer_tests.cpp",
        "dft_tests.cpp",
        "fft_tests.cpp",
        "gauss_filter_tests.cpp",
        "image_converter_tests.cpp",
        "label_marker_tests.cpp",
        "spectrum_shifter_tests.cpp",
    ]    
}
