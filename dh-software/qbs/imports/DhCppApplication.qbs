import qbs

CppApplication {
    property pathList additionalIncludePaths: []
    property pathList additionalLibraryPaths: []
    property stringList additionalStaticLibraries: []
    property stringList additionalCXXFlags: []

    cpp.cxxLanguageVersion: "c++17"
    cpp.cLanguageVersion: "c11"
    cpp.treatWarningsAsErrors: true

    cpp.cxxFlags: product.additionalCXXFlags.concat( project.additionalCXXFlags )
    cpp.includePaths: product.additionalIncludePaths.concat( project.additionalIncludePaths, product.sourceDirectory )
    cpp.libraryPaths: product.additionalLibraryPaths.concat( project.additionalLibraryPaths )
    cpp.staticLibraries: product.additionalStaticLibraries.concat([ "atomic" ])

    Group {
        qbs.install: true
        fileTagsFilter: product.type
        qbs.installDir: "bin"
    }
}
