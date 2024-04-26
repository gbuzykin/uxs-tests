#pragma once

#include "uxs/format.h"

#include <chrono>
#include <stdexcept>

#define ADD_TEST_CASE(cat, group, fn) \
    static uxs_test_suite::TestCase UXS_TOKENPASTE2(g_test_case_, __LINE__)(cat, group, fn)

#define VERIFY(...) \
    do { \
        if (!(__VA_ARGS__)) { \
            throw std::runtime_error(uxs_test_suite::report_error(__FILE__, __LINE__, #__VA_ARGS__)); \
        } \
    } while (false)

namespace uxs_test_suite {

using curr_clock_t = std::chrono::high_resolution_clock;
template<class Rep, class Period>
int64_t as_ns_duration(const std::chrono::duration<Rep, Period>& d) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(d).count();
}

std::string report_error(const char* file, int line, const char* msg);

struct TestCase {
    static const TestCase* first_avail;
    const TestCase* next_avail;
    TestCase(std::string cat, std::string group, int (*fn)())
        : next_avail(first_avail), category(std::move(cat)), group_name(std::move(group)), test(fn) {
        first_avail = this;
    }
    std::string category;
    std::string group_name;
    int (*test)();
};

}  // namespace uxs_test_suite
