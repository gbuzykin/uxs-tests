# A Test-Suite for Utilities and Extensions Library

It is a project for `std-ext` testing, which contains lots of use- and test-cases.  A simple own
test-suite is implemented here to sort and organize tests.  It consists of quick verification tests
with predefined input data, brute-force-like tests with large amount of random-generated data and
performance tests.  Tests from GCC test-suite are also ported for *containers* testing.

Each registered test has its *category* and *group* names.  Test-suite allows to specify substrings,
which are checked for occurrence to select tests to be included or excluded from testing session.

Normally, tests return zero in case of success and non-zero otherwise.  But for some special test
categories these return values are collected to be represented in the form of a table.  It's useful
for e.g. performance tests.

## How to Build `std-ext` Library and the Test-Suite

Perform this steps to build the project (in linux, for other platforms the steps are similar):

1. Clone `std-ext-tests` repository and enter it

    ```bash
    git clone https://github.com/gbuzykin/std-ext-tests
    cd std-ext-tests
    ```

2. Initialize and update `std-ext` submodule

    ```bash
    git submodule update --init
    ```

3. Then, compilation script should be created using `cmake` tool.  To use the default compiler just
   issue e.g.

    ```bash
    cmake -S . -B build
    ```

    or to make building scripts for debug or optimized configurations issue the following

    ```bash
    cmake -S . -B build -DCMAKE_BUILD_TYPE="Debug"
    ```

    or

    ```bash
    cmake -S . -B build -DCMAKE_BUILD_TYPE="Release"
    ```

4. Enter folder for building and run `make`

    ```bash
    cd build
    make
    ```

    to use several parallel processes (e.g. 8) for building run `make` with `-j` key

    ```bash
    make -j 8
    ```

## How to Modify Sets of Included and Excluded Test-Cases

Four vectors of strings are defined in `main.cpp` file located in `std-ext-tests/src/` folder of the
project. By default they are initialized with these sets:

```cpp
const std::vector<std::string_view> g_include_test_category = {"brute"};
const std::vector<std::string_view> g_exclude_test_category = {};

const std::vector<std::string_view> g_include_test_group = {};
const std::vector<std::string_view> g_exclude_test_group = {};
```

Strings in initializer lists are checked for occurrence in *category* and *group* strings to
determine which test-cases to include in the test session and which to exclude.  The most common
strings for *category* are `"brute"`, `"perf"` and `"info"` (quick verification tests are always
included).  The most common strings for *group* are `"string"`, `"format"`, `"vector"`, `"list"`,
`"list"`, `"rbtree"`, and others.

## How to Run Test

Just run resulting executable `std-ext-tests`.

Some tests can use several parallel threads.  To allow more than 1 thread use `-j` key, e.g.
`std-ext-tests -j 8`.

## References

This project uses [{fmt}](https://github.com/fmtlib/fmt) and some code from
[dtoa-benchmark](https://github.com/miloyip/dtoa-benchmark) as a reference for string formatting
results evaluation and performance comparison.  Also, the latter project has its
[fork](https://github.com/gbuzykin/dtoa-benchmark) to compare the performance results of `std-ext`
with other formatter's results.
