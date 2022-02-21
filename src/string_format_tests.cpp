#include "test_suite.h"
#include "util/format.h"

#include <cstdio>
#include <sstream>

namespace {

int test_string_format_0() {
    std::string s = util::format("ab{{c{:*<5.3}e}f}}g{", 14.567);
    s = util::format("{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234, 42, 3.13, "str", (void*)1000, 'X');
    // VERIFY(util::format("abc{}efg", 14) == "abc14efg");

    char buf[256];
    char* p = util::format_to(buf, "{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234, 42, 3.13, "str", (void*)1000, 'X');
    *p = '\0';

    std::ostringstream ss;
    ss << std::setprecision(10) << std::fixed << 1.234 << ':' << std::resetiosflags(std::ios::floatfield)
       << std::setw(4) << std::setfill('0') << 42 << std::setfill(' ') << ':' << std::setiosflags(std::ios::showpos)
       << 3.13 << std::resetiosflags(std::ios::showpos) << ':' << "str" << ':' << (void*)1000 << ':' << 'X' << ":%\n"
       << std::flush;
    std::string s2 = std::move(ss).str();
    return 0;
}

int test_string_format_1() {
    VERIFY(util::format("abcdefghi") == "abcdefghi");
    VERIFY(util::format("{}abcdefghi", "A") == "Aabcdefghi");
    VERIFY(util::format("{}abcdefghi{}", "A", "B") == "AabcdefghiB");
    VERIFY(util::format("{}{}abcdefghi{}", "A", "C", "B") == "ACabcdefghiB");

    VERIFY(util::format("{}{}{}{}", "1", "2", "3", "4") == "1234");
    VERIFY(util::format("{:*>8}", 1) == "*******1");
    VERIFY(util::format("{:*>8.2f}", 2.34) == "****2.34");
    VERIFY(util::format("{:.2f}", 2.34) == "2.34");

    return 0;
}

int perf(int iter_count) {
    int result = 0;
    char buf[128];

    auto start = std::clock();
    for (int iter = 0; iter < iter_count; ++iter) {
        char* p = util::format_to(buf, "{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234, 42, 3.13, "str", (void*)1000, 'X');
        result += static_cast<int>(p - buf);
    }

    return result != 0 ? static_cast<int>(std::clock() - start) : 0;
}

int perf_libc(int iter_count) {
    int result = 0;
    char buf[128];

    auto start = std::clock();
    for (int iter = 0; iter < iter_count; ++iter) {
        result += sprintf(buf, "%0.10f:%04d:%+g:%s:%p:%c:%%\n", 1.234, 42, 3.13, "str", (void*)1000, (int)'X');
    }

    return result != 0 ? static_cast<int>(std::clock() - start) : 0;
}

int perf_std(int iter_count) {
    int result = 0;

    auto start = std::clock();
    for (int iter = 0; iter < iter_count; ++iter) {
        std::ostringstream ss;
        ss << std::setprecision(10) << std::fixed << 1.234 << ':' << std::resetiosflags(std::ios::floatfield)
           << std::setw(4) << std::setfill('0') << 42 << std::setfill(' ') << ':' << std::setiosflags(std::ios::showpos)
           << 3.13 << std::resetiosflags(std::ios::showpos) << ':' << "str" << ':' << (void*)1000 << ':' << 'X'
           << ":%\n"
           << std::flush;
        std::string s = std::move(ss).str();
        result += static_cast<int>(s.size());
    }

    return result != 0 ? static_cast<int>(std::clock() - start) : 0;
}

const int perf_N = 2000000;
int test_perf() { return perf(perf_N); }
int test_perf_libc() { return perf_libc(perf_N); }
int test_perf_std() { return perf_std(perf_N); }

}  // namespace

ADD_TEST_CASE("", "string format", test_string_format_0);
ADD_TEST_CASE("", "string format", test_string_format_1);

ADD_TEST_CASE("2-perf", "string format", test_perf);
ADD_TEST_CASE("2-perf", "<libc> string format", test_perf_libc);
ADD_TEST_CASE("2-perf", "<std> string format", test_perf_std);
