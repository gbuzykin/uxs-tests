#include "list_tests.h"

#include "uxs/pool_allocator.h"

#include <list>

using namespace uxs_test_suite;

namespace {

template<typename ListType>
int perf(int iter_count) {
    using value_type = typename ListType::value_type;

    srand(0);

    auto start = std::clock();

    ListType l;
    const value_type val0 = 10;
    for (int iter = 0; iter < iter_count; ++iter) {
        int act = rand() % 83;
        if (act >= 0 && act < 10) {
            l.emplace(std::next(l.begin(), rand() % (l.size() + 1)), rand() % 100);
        } else if (act >= 10 && act < 20) {
            size_t n = rand() % (l.size() + 1);
            size_t count = 1 + rand() % 5;
            value_type val[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
            l.insert(std::next(l.begin(), n), val, val + count);
        } else if (act >= 20 && act < 30) {
            if (!l.empty()) { l.erase(std::next(l.begin(), rand() % l.size())); }
        } else if (act >= 30 && act < 40) {
            if (!l.empty()) {
                size_t n = rand() % l.size();
                size_t count = 1 + rand() % (l.size() - n);
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
            l.resize(rand() % 100);
        } else if (act == 82) {
            l.resize(rand() % 100, val0);
        }
    }

    return static_cast<int>(std::clock() - start);
}

const int perf_N = 4000000;

int test_perf_T_std_alloc() { return perf<uxs::list<T>>(perf_N); }
int test_perf_T_global_pool() { return perf<uxs::list<T, uxs::global_pool_allocator<T>>>(perf_N); }
int test_perf_T_pool() { return perf<uxs::list<T, uxs::pool_allocator<T>>>(perf_N); }
int test_perf_int_std_alloc() { return perf<uxs::list<int>>(4 * perf_N); }
int test_perf_int_global_pool() { return perf<uxs::list<int, uxs::global_pool_allocator<int>>>(4 * perf_N); }
int test_perf_int_pool() { return perf<uxs::list<int, uxs::pool_allocator<int>>>(4 * perf_N); }

int test_perf_T_std_alloc_std() { return perf<std::list<T>>(perf_N); }
int test_perf_T_global_pool_std() { return perf<std::list<T, uxs::global_pool_allocator<T>>>(perf_N); }
int test_perf_T_pool_std() { return perf<std::list<T, uxs::pool_allocator<T>>>(perf_N); }
int test_perf_int_std_alloc_std() { return perf<std::list<int>>(4 * perf_N); }
int test_perf_int_global_pool_std() { return perf<std::list<int, uxs::global_pool_allocator<int>>>(4 * perf_N); }
int test_perf_int_pool_std() { return perf<std::list<int, uxs::pool_allocator<int>>>(4 * perf_N); }

}  // namespace

ADD_TEST_CASE("2-perf", "list:T", test_perf_T_std_alloc);
ADD_TEST_CASE("2-perf", "list:T:global_pool_allocator", test_perf_T_global_pool);
ADD_TEST_CASE("2-perf", "list:T:pool_allocator", test_perf_T_pool);
ADD_TEST_CASE("2-perf", "list:int", test_perf_int_std_alloc);
ADD_TEST_CASE("2-perf", "list:int:global_pool_allocator", test_perf_int_global_pool);
ADD_TEST_CASE("2-perf", "list:int:pool_allocator", test_perf_int_pool);
ADD_TEST_CASE("2-perf", "<std> list:T", test_perf_T_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> list:T:global_pool_allocator", test_perf_T_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> list:T:pool_allocator", test_perf_T_pool_std);
ADD_TEST_CASE("2-perf", "<std> list:int", test_perf_int_std_alloc_std);
ADD_TEST_CASE("2-perf", "<std> list:int:global_pool_allocator", test_perf_int_global_pool_std);
ADD_TEST_CASE("2-perf", "<std> list:int:pool_allocator", test_perf_int_pool_std);
