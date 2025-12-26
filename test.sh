#!/bin/bash -e
C_COMPILER=${C_COMPILER:-cc}
CXX_COMPILER=${CXX_COMPILER:-c++}
CXX_STD=${CXX_STD:-20}
USE_LIBCPP=${USE_LIBCPP:-OFF}
BUILD_TYPE=${BUILD_TYPE:-Debug}
N_PROC=${N_PROC:-8}
BUILD_OPTS=$CXX_COMPILER-C++$CXX_STD
if [[ $CXX_COMPILER == clang* ]] && [[ $USE_LIBCPP == ON ]]; then BUILD_OPTS=$BUILD_OPTS-libc++; fi
BUILD_OPTS=$BUILD_OPTS-$BUILD_TYPE
BUILD_DIR=./build-test-$BUILD_OPTS
echo "---- Checking C++$CXX_STD $BUILD_TYPE compilation"
cmake -S . -B $BUILD_DIR -G "Ninja" \
  -DOPTION_TREAT_WARNINGS_AS_ERRORS=ON \
  -DCMAKE_C_COMPILER=$C_COMPILER \
  -DCMAKE_CXX_COMPILER=$CXX_COMPILER \
  -DCMAKE_CXX_STANDARD=$CXX_STD \
  -DUSE_LIBCPP=$USE_LIBCPP \
  -DCMAKE_BUILD_TYPE=$BUILD_TYPE
cmake --build $BUILD_DIR -j $N_PROC
if [[ $COMPILE_ONLY != ON ]]; then
  echo "---- Runing tests for C++$CXX_STD $BUILD_TYPE compilation"
  INSTALL_DIR=./install-test-$BUILD_OPTS
  cmake --install $BUILD_DIR --prefix $INSTALL_DIR
  $INSTALL_DIR/bin/uxs-tests -d testdata $*
fi
