import qbs

DhUnitTests {    
    name: "common_tests"

    Depends { name: "common" }

    files: [
        "dh_string_tests.cpp",
    ]
}
