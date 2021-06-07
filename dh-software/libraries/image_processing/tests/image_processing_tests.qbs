import qbs

DhUnitTests {
    name: "image_processing_tests"

    Depends { name: "image_processing" }

    files: [
        "dft_tests.cpp",
        "gabor_tests.cpp",
        "image_32fc_tests.cpp",
        "image_converter_tests.cpp",
        "image_processing_tests.cpp",
        "spectrum_shifter_tests.cpp",
    ]    
}
