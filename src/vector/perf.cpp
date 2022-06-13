#include "vector_tests.h"

#include <vector>

using namespace uxs_test_suite;

namespace {

template<typename VecType>
int perf(int iter_count) {
    using value_type = typename VecType::value_type;

    srand(0);

    auto start = std::clock();

    VecType v;
    const value_type val0 = 10;
    for (int iter = 0; iter < iter_count; ++iter) {
        int act = rand() % 64;
        if (act >= 0 && act < 10) {
            v.emplace(v.begin() + rand() % (v.size() + 1), val0);
        } else if (act >= 10 && act < 20) {
            size_t n = rand() % (v.size() + 1);
            size_t count = 1 + rand() % 5;
            value_type val[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
            v.insert(v.begin() + n, val, val + count);
        } else if (act >= 20 && act < 30) {
            if (!v.empty()) { v.erase(v.begin() + rand() % v.size()); }
        } else if (act >= 30 && act < 40) {
            if (!v.empty()) {
                size_t n = rand() % v.size();
                size_t count = 1 + rand() % (v.size() - n);
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
            v.resize(rand() % 100);
        } else if (act == 63) {
            v.resize(rand() % 100, val0);
        }
    }

    return static_cast<int>(std::clock() - start);
}

const int perf_N = 4000000;

int test_perf_T() { return perf<uxs::vector<T>>(perf_N); }
int test_perf_int() { return perf<uxs::vector<int>>(4 * perf_N); }
int test_perf_char() { return perf<uxs::vector<char>>(4 * perf_N); }

int test_perf_T_std() { return perf<std::vector<T>>(perf_N); }
int test_perf_int_std() { return perf<std::vector<int>>(4 * perf_N); }
int test_perf_char_std() { return perf<std::vector<char>>(4 * perf_N); }

}  // namespace

ADD_TEST_CASE("2-perf", "vector:T", test_perf_T);
ADD_TEST_CASE("2-perf", "vector:int", test_perf_int);
ADD_TEST_CASE("2-perf", "vector:char", test_perf_char);
ADD_TEST_CASE("2-perf", "<std> vector:T", test_perf_T_std);
ADD_TEST_CASE("2-perf", "<std> vector:int", test_perf_int_std);
ADD_TEST_CASE("2-perf", "<std> vector:char", test_perf_char_std);
