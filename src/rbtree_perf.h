#include "rbtree_tests.h"

#include <cmath>
#include <random>
#include <vector>

namespace uxs_test_suite {

template<typename SetType>
int perf_insert(int n_secs) {
    std::default_random_engine gen;
    std::uniform_int_distribution<int> d(0, 499);
    const int estimate_loop_count = 1000;

    std::vector<int> v;
    v.reserve(1000);
    for (size_t i = 0; i < 1000; ++i) { v.push_back(d(gen)); }

    auto start = curr_clock_t::now();

    SetType s;
    bool is_estimating = true;
    int n_loop = 0, loop_count = estimate_loop_count;
    while (n_loop < loop_count) {
        s.clear();
        for (int val : v) { s.emplace(val); }
        ++n_loop;
        if (is_estimating && n_loop == estimate_loop_count) {
            is_estimating = false, n_loop = 0;
            auto start1 = curr_clock_t::now();
            loop_count = static_cast<int>(
                std::ceil((estimate_loop_count * n_secs * 1000000000.0) / as_ns_duration(start1 - start)));
            start = start1;
        }
    }
    return static_cast<int>(1000 * as_ns_duration(curr_clock_t::now() - start) / loop_count);
}

template<typename SetType>
int perf_forward(int n_secs) {
    std::default_random_engine gen;
    std::uniform_int_distribution<int> d(0, 499);
    const int estimate_loop_count = 10000;

    SetType s;
    for (size_t i = 0; i < 1000; ++i) { s.emplace(d(gen)); }

    auto start = curr_clock_t::now();

    int result = 0;
    bool is_estimating = true;
    int n_loop = 0, loop_count = estimate_loop_count;
    while (n_loop < loop_count) {
        for (auto it = s.begin(); it != s.end(); ++it) { result += static_cast<int>(*it); }
        ++n_loop;
        if (is_estimating && n_loop == estimate_loop_count) {
            is_estimating = false, n_loop = 0;
            auto start1 = curr_clock_t::now();
            loop_count = static_cast<int>(
                std::ceil((estimate_loop_count * n_secs * 1000000000.0) / as_ns_duration(start1 - start)));
            start = start1;
        }
    }
    return result ? static_cast<int>(1000 * as_ns_duration(curr_clock_t::now() - start) / loop_count) : 0;
}

template<typename SetType>
int perf_backward(int n_secs) {
    std::default_random_engine gen;
    std::uniform_int_distribution<int> d(0, 499);
    const int estimate_loop_count = 10000;

    SetType s;
    for (size_t i = 0; i < 1000; ++i) { s.emplace(d(gen)); }

    auto start = curr_clock_t::now();

    int result = 0;
    bool is_estimating = true;
    int n_loop = 0, loop_count = estimate_loop_count;
    while (n_loop < loop_count) {
        for (auto it = s.rbegin(); it != s.rend(); ++it) { result += static_cast<int>(*it); }
        ++n_loop;
        if (is_estimating && n_loop == estimate_loop_count) {
            is_estimating = false, n_loop = 0;
            auto start1 = curr_clock_t::now();
            loop_count = static_cast<int>(
                std::ceil((estimate_loop_count * n_secs * 1000000000.0) / as_ns_duration(start1 - start)));
            start = start1;
        }
    }
    return result ? static_cast<int>(1000 * as_ns_duration(curr_clock_t::now() - start) / loop_count) : 0;
}

template<typename SetType>
int perf_find(int n_secs) {
    std::default_random_engine gen;
    std::uniform_int_distribution<int> d(0, 499);
    const int estimate_loop_count = 1000;

    SetType s;
    std::vector<int> v;
    v.reserve(1000);
    for (size_t i = 0; i < 1000; ++i) {
        s.emplace(d(gen));
        v.push_back(d(gen));
    }

    auto start = curr_clock_t::now();

    int result = 0;
    bool is_estimating = true;
    int n_loop = 0, loop_count = estimate_loop_count;
    while (n_loop < loop_count) {
        for (int val : v) {
            auto lower = s.lower_bound(val);
            auto upper = s.upper_bound(val);
            auto range = s.equal_range(val);

            result += lower != s.end() ? static_cast<int>(*lower) : 0;
            result += upper != s.end() ? static_cast<int>(*upper) : 0;
            result += range.first != s.end() ? static_cast<int>(*range.first) : 0;
            result += range.second != s.end() ? static_cast<int>(*range.second) : 0;
        }
        ++n_loop;
        if (is_estimating && n_loop == estimate_loop_count) {
            is_estimating = false, n_loop = 0;
            auto start1 = curr_clock_t::now();
            loop_count = static_cast<int>(
                std::ceil((estimate_loop_count * n_secs * 1000000000.0) / as_ns_duration(start1 - start)));
            start = start1;
        }
    }

    return result ? static_cast<int>(1000 * as_ns_duration(curr_clock_t::now() - start) / loop_count) : 0;
}

template<typename SetType>
int perf_integral(int n_secs) {
    std::default_random_engine gen;
    std::uniform_int_distribution<int> d(0, 499);
    const int estimate_loop_count = 100;

    std::vector<int> v1, v2;
    v1.reserve(1000);
    v2.reserve(1000);
    for (size_t i = 0; i < 1000; ++i) {
        v1.push_back(d(gen));
        v2.push_back(d(gen));
    }

    auto start = curr_clock_t::now();

    SetType s;
    int result = 0;
    bool is_estimating = true;
    int n_loop = 0, loop_count = estimate_loop_count;
    while (n_loop < loop_count) {
        s.clear();
        for (size_t i = 0; i < 1000; ++i) { s.emplace(v1[i]); }

        for (size_t i = 0; i < 30; ++i) {
            for (auto it = s.begin(); it != s.end(); ++it) { result += static_cast<int>(*it); }
            for (auto it = s.rbegin(); it != s.rend(); ++it) { result += static_cast<int>(*it); }
        }

        for (size_t i = 0; i < 1000; ++i) {
            int val = v2[i];

            auto lower = s.lower_bound(val);
            auto upper = s.upper_bound(val);
            auto range = s.equal_range(val);

            result += lower != s.end() ? static_cast<int>(*lower) : 0;
            result += upper != s.end() ? static_cast<int>(*upper) : 0;
            result += range.first != s.end() ? static_cast<int>(*range.first) : 0;
            result += range.second != s.end() ? static_cast<int>(*range.second) : 0;

            s.erase(lower, upper);
        }

        ++n_loop;
        if (is_estimating && n_loop == estimate_loop_count) {
            is_estimating = false, n_loop = 0;
            auto start1 = curr_clock_t::now();
            loop_count = static_cast<int>(
                std::ceil((estimate_loop_count * n_secs * 1000000000.0) / as_ns_duration(start1 - start)));
            start = start1;
        }
    }

    return result ? static_cast<int>(1000 * as_ns_duration(curr_clock_t::now() - start) / loop_count) : 0;
}

}  // namespace uxs_test_suite
