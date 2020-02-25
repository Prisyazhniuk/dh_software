import qbs

DhUnitTests {
    name: "dh_viewer_core_tests"

    Depends { name: "dh_viewer_core" }

    files: [
        "bresenham_line_algorithm_tests.cpp",
    ]

}
