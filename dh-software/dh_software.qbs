import qbs
import qbs.Process
import qbs.Environment

Project {
    name: "dh-software"
    qbsSearchPaths: "qbs"
    minimumQbsVersion: "1.12.2"

    references: [
        "applications/dh_viewer/dh_viewer.qbs",

        "libraries/common/source/common.qbs",
        "libraries/common/tests/common_tests.qbs",

        "libraries/qt_helpers/source/qt_helpers.qbs",
        //"libraries/qt_helpers/tests/qt_helpers_tests.qbs",

        "libraries/dh_viewer_core/source/dh_viewer_core.qbs",
        //"libraries/dh_viewer_core/tests/dh_viewer_core_tests.qbs",

        "libraries/image_processing/source/image_processing.qbs",
        "libraries/image_processing/tests/image_processing_tests.qbs",
    ]

    property string thirdparty_path: sourceDirectory + "/../3rdparty-mingw"
    property string opencv_bin_path: Environment.getEnv("OPENCV_ROOT")+"/x64/mingw/bin";
    property string ipp_bin_path: Environment.getEnv("IPP_ROOT")+"/../redist/intel64/ipp";

    property pathList additionalIncludePaths: [
        thirdparty_path+"/include",
        Environment.getEnv("OPENCV_ROOT")+"/include",
        Environment.getEnv("IPP_ROOT")+"/include"
    ]

    property pathList additionalLibraryPaths: [
        thirdparty_path+"/lib",
        Environment.getEnv("OPENCV_ROOT")+"/x64/mingw/lib",
        Environment.getEnv("IPP_ROOT")+"/lib/intel64"
    ]

    property stringList additionalCXXFlags: [
        "-Wno-unused-parameter",
        "-D_USE_MATH_DEFINES"
    ]

    AutotestRunner {}
}
