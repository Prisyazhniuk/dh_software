import qbs

DhUnitTests {
    name: "image_processing_tests"

    Depends { name: "image_processing" }

    files: [
        "image_converter_tests.cpp",
    ]    
}
