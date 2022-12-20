#if defined(_MSC_VER)
#    define _CRT_SECURE_NO_WARNINGS
#endif

#include "uxs/format.h"

#include "test_suite.h"

#include "uxs/guid.h"

#if !defined(_MSC_VER) || _MSC_VER >= 1920
#    include "fmt/format.h"
#endif

#if defined(_MSC_VER) && __cplusplus >= 201703L
#    include <charconv>
#endif

#include <cmath>
#include <cstdio>
#include <iomanip>
#include <sstream>

using namespace uxs_test_suite;

namespace {

int test_string_format_0() {
    VERIFY(uxs::format("{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234, 42, 3.13, "str", (void*)1000, 'X') ==
           "1.2340000000:0042:+3.13:str:0x3e8:X:%\n");
    return 0;
}

int test_string_format_1() {
    VERIFY(uxs::format("abcdefghi") == "abcdefghi");
    VERIFY(uxs::format("{}abcdefghi", "A") == "Aabcdefghi");
    VERIFY(uxs::format("{}abcdefghi{}", "A", "B") == "AabcdefghiB");
    VERIFY(uxs::format("{}{}abcdefghi{}", "A", "C", "B") == "ACabcdefghiB");

    VERIFY(uxs::format("{}{}{}{}", "1", "2", "3", "4") == "1234");
    VERIFY(uxs::format("{:*>8}", 1) == "*******1");
    VERIFY(uxs::format("{:*>8.2f}", 2.34) == "****2.34");
    VERIFY(uxs::format("{:.2f}", 2.34) == "2.34");

    VERIFY(uxs::format("{3}{2}{1}{0}", "1", "2", "3", "4") == "4321");
    VERIFY(uxs::format("{2}{1}{1}{0}", "1", "2", "3") == "3221");
    VERIFY(uxs::format("{2:=^{1}.{0}f}", 3, 7, 1.23) == "=1.230=");

    const char* s1 = "world";
    char* ncs1 = const_cast<char*>("world");
    std::string_view s2 = "world";
    std::string s3 = "world";
    VERIFY(uxs::format("Hello, {}!", "world") == "Hello, world!");
    VERIFY(uxs::format("Hello, {}!", s1) == "Hello, world!");
    VERIFY(uxs::format("Hello, {}!", ncs1) == "Hello, world!");
    VERIFY(uxs::format("Hello, {}!", s2) == "Hello, world!");
    VERIFY(uxs::format("Hello, {}!", s3) == "Hello, world!");
    VERIFY(uxs::format("Hello, {}!", s3 + "123") == "Hello, world123!");
    VERIFY(uxs::format("Hello, {}!", (void*)1000) == "Hello, 0x3e8!");
    VERIFY(uxs::format("Hello, {}!", (const void*)1000) == "Hello, 0x3e8!");
    VERIFY(uxs::format("Hello, {}!", (const int*)1000) == "Hello, 0x3e8!");

    VERIFY(uxs::format("{:.^15}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82.....");
    VERIFY(uxs::format("{:.^15.6}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82.....");
    VERIFY(uxs::format("{:.^15.8}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82.....");
    VERIFY(uxs::format("{:.^15.4}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           ".....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2......");
    VERIFY(uxs::format("{:.^15.0}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") == "...............");
    VERIFY(uxs::format("{:.6}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82");
    VERIFY(uxs::format("{:.4}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2");
    VERIFY(uxs::format("{:.0}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") == "");
    VERIFY(uxs::format("{:2.4}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2");

    VERIFY(uxs::format("{:.^15}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0....");
    VERIFY(uxs::format("{:.^15.6}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82.....");
    VERIFY(uxs::format("{:.^15.7}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0....");
    VERIFY(uxs::format("{:.^15.8}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0....");

    return 0;
}

int test_string_format_2() {
    uxs::guid id(0x17364152, 0x36b4, 0x4b3e, 0x81, 0xba, 0x5e, 0x79, 0xa6, 0x81, 0xba, 0xee);
    VERIFY(uxs::format("{}", id) == "{17364152-36B4-4B3E-81BA-5E79A681BAEE}");
    VERIFY(uxs::format("{:=^42}", id) == "=={17364152-36B4-4B3E-81BA-5E79A681BAEE}==");
    return 0;
}

ADD_TEST_CASE("", "string format", test_string_format_0);
ADD_TEST_CASE("", "string format", test_string_format_1);
ADD_TEST_CASE("", "string format", test_string_format_2);

//-----------------------------------------------------------------------------
// Performance tests

const int perf_N_secs = 5;

template<typename Func>
int perf_format_to_string(const Func& fn, int n_secs) {
    std::array<char, 128> buf;

    size_t len = 0;
    const int estimate_loop_count = 10000;
    const auto start0 = curr_clock_t::now();
    for (int i = 0; i < estimate_loop_count; ++i) {
        len = std::max<size_t>(len, fn(buf.data(), buf.data() + buf.size(), i));
    }
    const auto start = curr_clock_t::now();
    const int loop_count = static_cast<int>(
        std::ceil((estimate_loop_count * n_secs * 1000000000.0) / as_ns_duration(start - start0)));
    for (int i = 0; i < loop_count; ++i) {
        len = std::max<size_t>(len, fn(buf.data(), buf.data() + buf.size(), i % estimate_loop_count));
    }
    return len ? static_cast<int>(1000 * as_ns_duration(curr_clock_t::now() - start) / loop_count) : 0;
}

ADD_TEST_CASE("2-perf", "format string", ([]() {
                  return perf_format_to_string(
                      [](char* first, char* last, int i) {
                          return static_cast<size_t>(uxs::format_to(first, "{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234 * i,
                                                                    i, 3.13 * i, "str",
                                                                    reinterpret_cast<void*>(static_cast<uintptr_t>(i)),
                                                                    static_cast<char>('A' + i % 26)) -
                                                     first);
                      },
                      perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "<libc> format string", ([]() {
                  return perf_format_to_string(
                      [](char* first, char* last, int i) {
                          return std::sprintf(first, "%0.10f:%04d:%+g:%s:%p:%c:%%\n", 1.234 * i, i, 3.13 * i, "str",
                                              reinterpret_cast<void*>(static_cast<uintptr_t>(i)),
                                              static_cast<char>('A' + i % 26));
                      },
                      perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "<std::ostringstream> format string", ([]() {
                  return perf_format_to_string(
                      [](char* first, char* last, int i) {
                          std::ostringstream ss;
                          ss << std::setprecision(10) << std::fixed << 1.234 * i << ':'
                             << std::resetiosflags(std::ios::floatfield) << std::setw(4) << std::setfill('0') << i
                             << std::setfill(' ') << ':' << std::setiosflags(std::ios::showpos) << 3.13 * i
                             << std::resetiosflags(std::ios::showpos) << ':' << "str" << ':'
                             << reinterpret_cast<void*>(static_cast<uintptr_t>(i)) << ':'
                             << static_cast<char>('A' + i % 26) << ":%\n"
                             << std::flush;
                          std::string s = std::move(ss).str();
                          return s.size();
                      },
                      perf_N_secs);
              }));
#if defined(_MSC_VER) && __cplusplus > 201703L
ADD_TEST_CASE("2-perf", "<c++17/20> format string", ([]() {
                  return perf_format_to_string(
                      [](char* first, char* last, int i) {
                          return static_cast<size_t>(std::format_to(first, "{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234 * i,
                                                                    i, 3.13 * i, "str",
                                                                    reinterpret_cast<void*>(static_cast<uintptr_t>(i)),
                                                                    static_cast<char>('A' + i % 26)) -
                                                     first);
                      },
                      perf_N_secs);
              }));
#endif
#if !defined(_MSC_VER) || _MSC_VER >= 1920
ADD_TEST_CASE("2-perf", "<{fmt}> format string", ([]() {
                  return perf_format_to_string(
                      [](char* first, char* last, int i) {
                          return static_cast<size_t>(fmt::format_to(first, "{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234 * i,
                                                                    i, 3.13 * i, "str",
                                                                    reinterpret_cast<void*>(static_cast<uintptr_t>(i)),
                                                                    static_cast<char>('A' + i % 26)) -
                                                     first);
                      },
                      perf_N_secs);
              }));
#endif

}  // namespace
