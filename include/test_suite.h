#pragma once

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#define ADD_TEST_CASE(cat, group, fn) static util_test_suite::TestCase g_test_case_##fn(cat, group, fn)

#define VERIFY(...) \
    do { \
        if (!(__VA_ARGS__)) { \
            throw std::runtime_error(util_test_suite::report_error(__FILE__, __LINE__, #__VA_ARGS__)); \
        } \
    } while (false)

namespace util_test_suite {

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

}  // namespace util_test_suite
