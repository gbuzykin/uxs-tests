#!/bin/bash -e
BUILD_TYPE=${BUILD_TYPE:-Debug}
N_PROC=${N_PROC:-8}
CXX_COMPILER=g++-12 CXX_STD=11 ./test.sh
CXX_COMPILER=g++-12 CXX_STD=14 ./test.sh
CXX_COMPILER=g++-12 CXX_STD=17 ./test.sh
CXX_COMPILER=g++-12 CXX_STD=20 ./test.sh
CXX_COMPILER=g++-12 CXX_STD=23 ./test.sh
CXX_COMPILER=clang++-17 USE_LIBCPP=False CXX_STD=11 ./test.sh
CXX_COMPILER=clang++-17 USE_LIBCPP=False CXX_STD=14 ./test.sh
CXX_COMPILER=clang++-17 USE_LIBCPP=False CXX_STD=17 ./test.sh
CXX_COMPILER=clang++-17 USE_LIBCPP=False CXX_STD=20 ./test.sh
CXX_COMPILER=clang++-17 USE_LIBCPP=False CXX_STD=23 ./test.sh
CXX_COMPILER=clang++-17 USE_LIBCPP=True CXX_STD=11 ./test.sh
CXX_COMPILER=clang++-17 USE_LIBCPP=True CXX_STD=14 ./test.sh
CXX_COMPILER=clang++-17 USE_LIBCPP=True CXX_STD=17 ./test.sh
CXX_COMPILER=clang++-17 USE_LIBCPP=True CXX_STD=20 ./test.sh
CXX_COMPILER=clang++-17 USE_LIBCPP=True CXX_STD=23 ./test.sh
CXX_COMPILER=clang++-17 USE_LIBCPP=True CXX_STD=20 BUILD_TYPE=Debug ./test.sh --include-category brute -j $N_PROC
CXX_COMPILER=clang++-17 USE_LIBCPP=True CXX_STD=20 BUILD_TYPE=Release ./test.sh --include-category perf
echo "---- SUCCESS"
