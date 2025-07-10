#include "list_tests.h"

#include <uxs-legacy/pool_allocator.h>

#include <cmath>
#include <list>
#include <random>

using namespace uxs_test_suite;

namespace {

const int perf_N_secs = 5;

template<typename ListType>
int perf(int n_secs) {
    using value_type = typename ListType::value_type;

    std::default_random_engine gen;
    std::uniform_int_distribution<int> d(0, std::numeric_limits<int>::max());
    const int estimate_loop_count = 10000;

    auto start = curr_clock_t::now();

    ListType l;
    const value_type val0 = 10;
    bool is_estimating = true;
    int n_loop = 0, loop_count = estimate_loop_count;
    while (n_loop < loop_count) {
        int act = d(gen) % 83;
        if (act >= 0 && act < 10) {
            l.emplace(std::next(l.begin(), d(gen) % (l.size() + 1)), d(gen) % 100);
        } else if (act >= 10 && act < 20) {
            size_t n = d(gen) % (l.size() + 1);
            size_t count = 1 + d(gen) % 5;
            value_type val[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
            l.insert(std::next(l.begin(), n), val, val + count);
        } else if (act >= 20 && act < 30) {
            if (!l.empty()) { l.erase(std::next(l.begin(), d(gen) % l.size())); }
        } else if (act >= 30 && act < 40) {
            if (!l.empty()) {
                size_t n = d(gen) % l.size();
                size_t count = 1 + d(gen) % (l.size() - n);
                l.erase(std::next(l.begin(), n), std::next(l.begin(), n + count));
            }
        } else if (act >= 40 && act < 50) {
            l.emplace_back(val0);
        } else if (act >= 50 && act < 60) {
            if (!l.empty()) { l.pop_back(); }
        } else if (act >= 60 && act < 70) {
            l.emplace_front(val0);
        } else if (act >= 70 && act < 80) {
            if (!l.empty()) { l.pop_front(); }
        } else if (act == 80) {
            l.clear();
        } else if (act == 81) {
            l.resize(d(gen) % 100);
        } else if (act == 82) {
            l.resize(d(gen) % 100, val0);
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

ADD_TEST_CASE("2-perf", "list:T", []() { return perf<uxs::list<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "list:T:global_pool_allocator",
              ([]() { return perf<uxs::list<T, uxs::global_pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "list:T:pool_allocator",
              ([]() { return perf<uxs::list<T, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "list:int", []() { return perf<uxs::list<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "list:int:global_pool_allocator",
              ([]() { return perf<uxs::list<int, uxs::global_pool_allocator<int>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "list:int:pool_allocator",
              ([]() { return perf<uxs::list<int, uxs::pool_allocator<int>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> list:T", []() { return perf<std::list<T>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "<STL> list:T:global_pool_allocator",
              ([]() { return perf<std::list<T, uxs::global_pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> list:T:pool_allocator",
              ([]() { return perf<std::list<T, uxs::pool_allocator<T>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> list:int", []() { return perf<std::list<int>>(perf_N_secs); });
ADD_TEST_CASE("2-perf", "<STL> list:int:global_pool_allocator",
              ([]() { return perf<std::list<int, uxs::global_pool_allocator<int>>>(perf_N_secs); }));
ADD_TEST_CASE("2-perf", "<STL> list:int:pool_allocator",
              ([]() { return perf<std::list<int, uxs::pool_allocator<int>>>(perf_N_secs); }));

}  // namespace
