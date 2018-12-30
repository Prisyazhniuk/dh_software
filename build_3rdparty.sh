#!/bin/bash

fail()
{
    read -p "Press enter to close"
    exit -1
}

# Dependences
cmake --version &> /dev/null
if [ $? != 0 ]; then echo "cmake not available"; fail; fi

# Destination
PREFIX="3rdparty-mingw"
PREFIX=`readlink -m $PREFIX`
mkdir -p "$PREFIX"

BUILD_PREFIX="build-3rdparty"
BUILD_PREFIX=`readlink -m $BUILD_PREFIX`

echo "prefix: $PREFIX"
echo "build prefix: $BUILD_PREFIX"

cd 3rdparty

build_googletest()
{
    # googletest
    echo "build googletest"

    PWD_BACKUP=$(pwd)
    cd googletest
    if [ $? != 0 ]; then return -1; fi

    SOURCES_DIR=$(pwd)  
    BUILD_DIR="$BUILD_PREFIX/googletest"

    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    if [ $? != 0 ]; then return -1; fi

    eval cmake                                  \
        "$SOURCES_DIR"                          \
        -DCMAKE_INSTALL_PREFIX:PATH="$PREFIX"   \
        "-G \"MSYS Makefiles\""                 \
        "-Dgtest_disable_pthreads=ON"           \
        -DBUILD_GMOCK=TRUE

    if [ $? != 0 ]; then return -1; fi

    make -j 4
    if [ $? != 0 ]; then return -1; fi

    make install
    if [ $? != 0 ]; then return -1; fi

    cd $PWD_BACKUP
    return 0
}

build_googletest
if [ $? != 0 ]; then fail; fi

echo "build complete ($SECONDS seconds)"
read -p "Press enter to close"
