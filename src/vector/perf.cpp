#include "vector_tests.h"

#include <cmath>
#include <random>
#include <vector>

using namespace uxs_test_suite;

namespace {

const int perf_N_secs = 5;

template<typename VecType>
int perf(int n_secs) {
    using value_type = typename VecType::value_type;

    std::default_random_engine gen;
    std::uniform_int_distribution<int> d(0, std::numeric_limits<int>::max());
    const int estimate_loop_count = 10000;

    auto start = curr_clock_t::now();

    VecType v;
    const value_type val0 = 10;
    bool is_estimating = true;
    int n_loop = 0, loop_count = estimate_loop_count;
    while (n_loop < loop_count) {
        int act = d(gen) % 64;
        if (act >= 0 && act < 10) {
            v.emplace(v.begin() + d(gen) % (v.size() + 1), val0);
        } else if (act >= 10 && act < 20) {
            size_t n = d(gen) % (v.size() + 1);
            size_t count = 1 + d(gen) % 5;
            value_type val[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
            v.insert(v.begin() + n, val, val + count);
        } else if (act >= 20 && act < 30) {
            if (!v.empty()) { v.erase(v.begin() + d(gen) % v.size()); }
        } else if (act >= 30 && act < 40) {
            if (!v.empty()) {
                size_t n = d(gen) % v.size();
                size_t count = 1 + d(gen) % (v.size() - n);
                v.erase(v.begin() + n, v.begin() + n + count);
            }
        } else if (act >= 40 && act < 50) {
            v.emplace_back(val0);
        } else if (act >= 50 && act < 60) {
            if (!v.empty()) { v.pop_back(); }
        } else if (act == 60) {
            v.shrink_to_fit();
        } else if (act == 61) {
            v.clear();
        } else if (act == 62) {
            v.resize(d(gen) % 100);
        } else if (act == 63) {
            v.resize(d(gen) % 100, val0);
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

    return static_cast<int>(1000 * as_ns_duration(curr_clock_t::now() - start) / loop_count);
}

ADD_TEST_CASE("2-perf", "vector:T", []() { return perf<uxs::vector<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "vector:int", []() { return perf<uxs::vector<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "vector:char", []() { return perf<uxs::vector<char>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "<STL> vector:T", []() { return perf<std::vector<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "<STL> vector:int", []() { return perf<std::vector<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "<STL> vector:char", []() { return perf<std::vector<char>>(perf_N_secs); });

}  // namespace
