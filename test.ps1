if (!$C_COMPILER) { $C_COMPILER="cl" }
if (!$CXX_COMPILER) { $CXX_COMPILER="cl" }
if (!$CXX_STD) { $CXX_STD=20 }
if (!$BUILD_TYPE) { $BUILD_TYPE="Debug" }
if (!$N_PROC) { $N_PROC=4 }
$USE_SANITIZERS = "clang-cl" -ne $CXX_COMPILER ? "ON" : "OFF"
$BUILD_OPTS="$CXX_COMPILER-C++$CXX_STD-$BUILD_TYPE"
echo "---- Checking C++$CXX_STD $BUILD_TYPE compilation"
$BUILD_DIR="./build-test-$BUILD_OPTS"
cmake -S . -B $BUILD_DIR -G "Ninja" `
  -DOPTION_TREAT_WARNINGS_AS_ERRORS="ON" `
  -DCMAKE_C_COMPILER="$C_COMPILER" `
  -DCMAKE_CXX_COMPILER="$CXX_COMPILER" `
  -DCMAKE_CXX_STANDARD="$CXX_STD" `
  -DCMAKE_BUILD_TYPE="$BUILD_TYPE" `
  -DUSE_SANITIZERS_FOR_DEBUG="$USE_SANITIZERS"
if ($LastExitCode) { Exit -1 }
cmake --build $BUILD_DIR -j $N_PROC
if ($LastExitCode) { Exit -1 }
if ("ON" -ne $COMPILE_ONLY) {
  echo "---- Runing tests for C++$CXX_STD $BUILD_TYPE compilation"
  $INSTALL_DIR="./install-test-$BUILD_OPTS"
  cmake --install $BUILD_DIR --prefix $INSTALL_DIR
  & "$INSTALL_DIR/bin/uxs-tests.exe" -d testdata $Args
  if ($LastExitCode) { Exit -1 }
}
