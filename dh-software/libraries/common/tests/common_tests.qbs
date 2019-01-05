import qbs

DhUnitTests {    
    name: "common_tests"

    Depends { name: "common" }

    files: [
        "dh_math_tests.cpp",
        "dh_string_tests.cpp",
    ]
}
