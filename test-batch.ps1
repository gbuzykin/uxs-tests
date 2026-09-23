if (!$BUILD_TYPE) { $BUILD_TYPE="Debug" }
if (!$N_PROC) { $N_PROC=8 }
$OLD_C_COMPILER=$C_COMPILER
$OLD_CXX_COMPILER=$CXX_COMPILER

$BUILD_TYPE="Debug"; $CXX_STD=14; $BUILD_STATIC="OFF"; ./test.ps1
if ($LastExitCode) { Exit -1 }
$BUILD_TYPE="Debug"; $CXX_STD=17; $BUILD_STATIC="OFF"; ./test.ps1
if ($LastExitCode) { Exit -1 }
$BUILD_TYPE="Debug"; $CXX_STD=20; $BUILD_STATIC="ON"; ./test.ps1
if ($LastExitCode) { Exit -1 }
$BUILD_TYPE="Debug"; $CXX_STD=23; $BUILD_STATIC="OFF"; ./test.ps1
if ($LastExitCode) { Exit -1 }
$BUILD_TYPE="Release"; $CXX_STD=20; $BUILD_STATIC="ON"; ./test.ps1
if ($LastExitCode) { Exit -1 }

$C_COMPILER="clang-cl"
$CXX_COMPILER="clang-cl"

$BUILD_TYPE="Debug"; $CXX_STD=14; $BUILD_STATIC="OFF"; ./test.ps1
if ($LastExitCode) { Exit -1 }
$BUILD_TYPE="Debug"; $CXX_STD=17; $BUILD_STATIC="OFF"; ./test.ps1
if ($LastExitCode) { Exit -1 }
$BUILD_TYPE="Debug"; $CXX_STD=20; $BUILD_STATIC="ON"; ./test.ps1
if ($LastExitCode) { Exit -1 }
$BUILD_TYPE="Debug"; $CXX_STD=23; $BUILD_STATIC="OFF"; ./test.ps1
if ($LastExitCode) { Exit -1 }
$BUILD_TYPE="Release"; $CXX_STD=20; $BUILD_STATIC="ON"; ./test.ps1
if ($LastExitCode) { Exit -1 }

$C_COMPILER=$OLD_C_COMPILER
$CXX_COMPILER=$OLD_CXX_COMPILER

$BUILD_TYPE="Release"; $CXX_STD=20; $BUILD_STATIC="OFF"; ./test.ps1 --include-category perf
if ($LastExitCode) { Exit -1 }
$BUILD_TYPE="Debug"; $CXX_STD=20; $BUILD_STATIC="OFF"; ./test.ps1 --include-category brute -j $N_PROC
if ($LastExitCode) { Exit -1 }

echo "---- SUCCESS"
