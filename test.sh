#!/bin/bash -e
CXX_COMPILER=${CXX_COMPILER:-g++}
CXX_STD=${CXX_STD:-20}
BUILD_TYPE=${BUILD_TYPE:-Debug}
N_PROC=${N_PROC:-8}
USE_LIBCPP=${USE_LIBCPP:-True}
BUILD_DIR=./build-test-$CXX_COMPILER-C++$CXX_STD-$BUILD_TYPE
if [[ $CXX_COMPILER == clang* ]]; then BUILD_DIR=$BUILD_DIR-libc++-$USE_LIBCPP; fi
echo "---- Checking C++$CXX_STD $BUILD_TYPE compilation"
mkdir -p $BUILD_DIR
sed -i "s/CMAKE_CXX_STANDARD [1-9][0-9]/CMAKE_CXX_STANDARD $CXX_STD/" CMakeLists.txt
cmake -S . -B $BUILD_DIR -G "Ninja" -DCMAKE_CXX_COMPILER=$CXX_COMPILER -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DUSE_ZLIB=True -DUSE_LIBZIP=True -DUSE_LIBCPP=$USE_LIBCPP
cmake --build $BUILD_DIR -j $N_PROC
echo "---- Runing tests for C++$CXX_STD $BUILD_TYPE compilation"
$BUILD_DIR/uxs-tests -d testdata $*
