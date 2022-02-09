#pragma once

#include "test_allocators.h"
#include "test_types.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#define ADD_TEST_CASE(cat, group, fn) static TestCase g_test_case_##fn(cat, group, fn)

#define VERIFY(...) \
    if (!(__VA_ARGS__)) { throw std::logic_error(report_error(__FILE__, __LINE__, #__VA_ARGS__)); }

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
