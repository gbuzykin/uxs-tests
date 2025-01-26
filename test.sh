#!/bin/bash -e
C_COMPILER=${C_COMPILER:-cc}
CXX_COMPILER=${CXX_COMPILER:-c++}
CXX_STD=${CXX_STD:-20}
USE_LIBCPP=${USE_LIBCPP:-OFF}
BUILD_TYPE=${BUILD_TYPE:-Debug}
N_PROC=${N_PROC:-8}
BUILD_DIR=./build-test-$CXX_COMPILER-C++$CXX_STD
if [[ $CXX_COMPILER == clang* ]] && [[ $USE_LIBCPP == ON ]]; then BUILD_DIR=$BUILD_DIR-libc++; fi
BUILD_DIR=$BUILD_DIR-$BUILD_TYPE
echo "---- Checking C++$CXX_STD $BUILD_TYPE compilation"
mkdir -p $BUILD_DIR
sed -i "s/CMAKE_CXX_STANDARD [1-9][0-9]/CMAKE_CXX_STANDARD $CXX_STD/" CMakeLists.txt
cmake -S . -B $BUILD_DIR -G "Ninja" \
  -DCMAKE_C_COMPILER=$C_COMPILER \
  -DCMAKE_CXX_COMPILER=$CXX_COMPILER \
  -DUSE_LIBCPP=$USE_LIBCPP \
  -DCMAKE_BUILD_TYPE=$BUILD_TYPE
cmake --build $BUILD_DIR -j $N_PROC
if [[ $COMPILE_ONLY != ON ]]; then
  echo "---- Runing tests for C++$CXX_STD $BUILD_TYPE compilation"
  $BUILD_DIR/uxs-tests -d testdata $*
fi
