import qbs

DhUnitTests {
    name: "image_processing_tests"

    Depends { name: "image_processing" }

    files: [
        "dft_tests.cpp",
        "fft_tests.cpp",
        "image_converter_tests.cpp",
        "spectrum_shifter_tests.cpp",
    ]    
}
