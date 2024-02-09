if (!$CXX_STD) { $CXX_STD=20 }
if (!$BUILD_TYPE) { $BUILD_TYPE="Debug" }
if (!$N_PROC) { $N_PROC=8 }
$BUILD_DIR="./build-test-C++$CXX_STD-$BUILD_TYPE"
echo "---- Checking C++$CXX_STD $BUILD_TYPE compilation"
New-item $BUILD_DIR -ItemType Directory -Force > $null
(Get-Content CMakeLists.txt) -replace "CMAKE_CXX_STANDARD [1-9][0-9]", "CMAKE_CXX_STANDARD $CXX_STD" | Out-File CMakeLists.txt -encoding ASCII
cmake -S . -B $BUILD_DIR -G "Ninja" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DUSE_ZLIB=True -DUSE_LIBZIP=True
if ($LastExitCode) { Exit -1 }
cmake --build $BUILD_DIR -j $N_PROC
if ($LastExitCode) { Exit -1 }
echo "---- Runing tests for C++$CXX_STD $BUILD_TYPE compilation"
& "$BUILD_DIR/uxs-tests.exe" -d testdata $Args
if ($LastExitCode) { Exit -1 }
