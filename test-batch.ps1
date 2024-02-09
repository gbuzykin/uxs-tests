if (!$BUILD_TYPE) { $BUILD_TYPE="Debug" }
if (!$N_PROC) { $N_PROC=8 }
$CXX_STD=14; ./test.ps1
if ($LastExitCode) { Exit -1 }
$CXX_STD=17; ./test.ps1
if ($LastExitCode) { Exit -1 }
$CXX_STD=20; ./test.ps1
if ($LastExitCode) { Exit -1 }
$CXX_STD=20; $BUILD_TYPE="Debug"; ./test.ps1 --include-category brute -j $N_PROC
if ($LastExitCode) { Exit -1 }
$CXX_STD=20; $BUILD_TYPE="Release"; ./test.ps1 --include-category perf
if ($LastExitCode) { Exit -1 }
echo "---- SUCCESS"
