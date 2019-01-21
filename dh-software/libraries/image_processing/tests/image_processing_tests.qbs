import qbs

DhUnitTests {
    name: "image_processing_tests"

    Depends { name: "image_processing" }

    files: [
        "fft_tests.cpp",
        "image_converter_tests.cpp",
    ]    
}
