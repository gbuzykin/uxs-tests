if (!$BUILD_TYPE) { $BUILD_TYPE="Debug" }
if (!$N_PROC) { $N_PROC=8 }
$OLD_BUILD_TYPE=$BUILD_TYPE
$OLD_C_COMPILER=$C_COMPILER
$OLD_CXX_COMPILER=$CXX_COMPILER

$CXX_STD=14; ./test.ps1
if ($LastExitCode) { Exit -1 }
$CXX_STD=17; ./test.ps1
if ($LastExitCode) { Exit -1 }
$CXX_STD=20; ./test.ps1
if ($LastExitCode) { Exit -1 }
$CXX_STD=23; ./test.ps1
if ($LastExitCode) { Exit -1 }
$BUILD_TYPE="Release"; $CXX_STD=20; ./test.ps1
if ($LastExitCode) { Exit -1 }

$BUILD_TYPE=$OLD_BUILD_TYPE
$C_COMPILER="clang-cl"
$CXX_COMPILER="clang-cl"

$CXX_STD=14; ./test.ps1
if ($LastExitCode) { Exit -1 }
$CXX_STD=17; ./test.ps1
if ($LastExitCode) { Exit -1 }
$CXX_STD=20; ./test.ps1
if ($LastExitCode) { Exit -1 }
$CXX_STD=23; ./test.ps1
if ($LastExitCode) { Exit -1 }
$BUILD_TYPE="Release"; $CXX_STD=20; ./test.ps1
if ($LastExitCode) { Exit -1 }

$BUILD_TYPE=$OLD_BUILD_TYPE
$C_COMPILER=$OLD_C_COMPILER
$CXX_COMPILER=$OLD_CXX_COMPILER

$BUILD_TYPE="Release"; $CXX_STD=20; ./test.ps1 --include-category perf
if ($LastExitCode) { Exit -1 }
$BUILD_TYPE="Debug"; $CXX_STD=20; ./test.ps1 --include-category brute -j $N_PROC
if ($LastExitCode) { Exit -1 }

echo "---- SUCCESS"
