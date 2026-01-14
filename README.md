# A Test-Suite for Utilities and eXtensionS (UXS) Library

It is a project for `uxs` testing, which contains lots of use- and test-cases. A simple own test-suite is implemented
here to sort and organize tests. It consists of quick verification tests with predefined input data, brute-force-like
tests with large amount of random-generated data and performance tests. Tests from GCC test-suite are also ported for
*containers* testing.

Each registered test has its *category* and *group* names. Test-suite allows to specify substrings, which are checked
for occurrence to select tests to be included or excluded from testing session.

Normally, tests return zero in case of success and non-zero otherwise. But for some special test categories these return
values are collected to be represented in the form of a table. It's useful for e.g. performance tests.

## How to Build `uxs` Library and the Test-Suite

Perform these steps to build the project:

1. Clone `uxs-tests` repository and enter it

    ```bash
    $ git clone https://github.com/gbuzykin/uxs-tests.git
    $ cd uxs-tests
    ```

2. Initialize and update `uxs` submodule

    ```bash
    $ git submodule update --init
    ```

3. Create compilation script using `cmake` tool

    ```bash
    $ cmake --preset default
    ```
   
    Default C & C++ compilers will be used.

4. Build tests

    ```bash
    $ cmake --build build --config Debug
    ```

    To run several parallel processes (e.g. 8) for building use `-j` key

    ```bash
    $ cmake --build build --config Debug -j 8
    ```

5. Install tests

    ```bash
    $ cmake --install build --config Debug --prefix <install-dir>
    ```

## How to Run Test

Just run resulting executable

```bash
$ <install-dir>/bin/uxs-tests -d testdata
```

## How to Modify Sets of Included and Excluded Test-Cases

Four vectors of strings can be specified using the following command line parameters:

1. `--include-category <category>...` - Include test categories.
2. `--exclude-category <category>...` - Exclude test categories.
3. `--include-group <group>...` - Include test groups.
4. `--exclude-group <group>...` - Exclude test groups.

Strings are checked for occurrence in *category* and *group* strings of test-cases to decide which ones to include in
the test session and which to exclude. The most common strings for *category* are `"brute"`, `"perf"`, and `"info"`
(quick verification tests are always included). The most common strings for *group* are `"string"`, `"format"`,
`"iobuf"`, `"json"`, `"xml"`, and others.

Some tests can use several parallel threads. To allow 2 or more threads use `-j` key, e.g. `-j 8`.

## References

This project uses [{fmt}](https://github.com/fmtlib/fmt) and some code from
[dtoa-benchmark](https://github.com/miloyip/dtoa-benchmark) as a reference for string formatting results evaluation and
performance comparison. Also, the latter project has its [fork](https://github.com/gbuzykin/dtoa-benchmark) to compare
the performance results of `uxs` with other formatter's results.
