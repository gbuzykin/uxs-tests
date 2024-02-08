#!/bin/bash -e
CXX_COMPILER=${CXX_COMPILER:-g++}
CXX_STD=${CXX_STD:-20}
BUILD_TYPE=${BUILD_TYPE:-Debug}
USE_LIBCPP=${USE_LIBCPP:-True}
BUILD_DIR=./build-$CXX_COMPILER-cxx$CXX_STD-$BUILD_TYPE
if [[ $CXX_COMPILER == clang* ]]; then BUILD_DIR=$BUILD_DIR-libcxx-$USE_LIBCPP; fi
echo "---- Checking C++$CXX_STD $BUILD_TYPE compilation"
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
sed -i "s/CMAKE_CXX_STANDARD [1-9][0-9]/CMAKE_CXX_STANDARD $CXX_STD/" CMakeLists.txt
cmake -S. -B$BUILD_DIR -DCMAKE_CXX_COMPILER=$CXX_COMPILER -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DUSE_ZLIB=True -DUSE_LIBZIP=True -DUSE_LIBCPP=$USE_LIBCPP
make -C$BUILD_DIR -j12
echo "---- Runing tests for C++$CXX_STD $BUILD_TYPE compilation"
$BUILD_DIR/uxs-tests -d testdata
