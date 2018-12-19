import qbs

StaticLibrary {
    property pathList additionalIncludePaths: []
    property stringList additionalCXXFlags: []

    Depends { name: "cpp" }
    cpp.cxxLanguageVersion: "c++17"
    cpp.cLanguageVersion: "c11"

    cpp.treatWarningsAsErrors: true
    cpp.cxxFlags: product.additionalCXXFlags.concat( project.additionalCXXFlags  )
    cpp.includePaths: product.additionalIncludePaths.concat( project.additionalIncludePaths, product.sourceDirectory )

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [ product.sourceDirectory ]
    }
}

