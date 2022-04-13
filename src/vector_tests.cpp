#include "vector_tests.h"

#include <vector>

using namespace util_test_suite;

namespace {

template<typename VecType>
void vector_test(int iter_count, bool log = false) {
    using value_type = typename VecType::value_type;

    VecType v;
    std::vector<value_type> v_ref;

    srand(0);

    if (log) { util::stdbuf::out.endl(); }

    for (int iter = 0, perc0 = -1; iter < iter_count; ++iter) {
        if (!log) {
            int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
            if (perc > perc0) {
                util::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
                perc0 = perc;
            }
        }

        int act = rand() % 65;
        if (act >= 0 && act < 10) {
            if (log) { util::println("insert one"); }

            size_t n = rand() % (v.size() + 1);
            char val = rand() % 100;
            auto t_res = v.emplace(v.begin() + n, val);
            auto ref_res = v_ref.emplace(v_ref.begin() + n, val);
            VERIFY(std::distance(v.begin(), t_res) == std::distance(v_ref.begin(), ref_res));
        } else if (act >= 10 && act < 20) {
            if (log) { util::println("insert"); }

            size_t n = rand() % (v.size() + 1);
            size_t count = 1 + rand() % 5;

            value_type val[10];
            for (size_t i = 0; i < count; ++i) { val[i] = rand() % 100; }

            auto t_res = v.insert(v.begin() + n, val, val + count);
            auto ref_res = v_ref.insert(v_ref.begin() + n, val, val + count);
            VERIFY(std::distance(v.begin(), t_res) == std::distance(v_ref.begin(), ref_res));
        } else if (act >= 20 && act < 30) {
            if (!v.empty()) {
                if (log) { util::println("erase one"); }

                size_t n = rand() % v.size();
                auto t_res = v.erase(v.begin() + n);
                auto ref_res = v_ref.erase(v_ref.begin() + n);
                VERIFY(std::distance(v.begin(), t_res) == std::distance(v_ref.begin(), ref_res));
            }
        } else if (act >= 30 && act < 40) {
            if (!v.empty()) {
                if (log) { util::println("erase"); }

                size_t n = rand() % (v.size() + 1);
                size_t count = rand() % (v.size() + 1 - n);
                auto t_res = v.erase(v.begin() + n, v.begin() + n + count);
                auto ref_res = v_ref.erase(v_ref.begin() + n, v_ref.begin() + n + count);
                VERIFY(std::distance(v.begin(), t_res) == std::distance(v_ref.begin(), ref_res));
            }
        } else if (act >= 40 && act < 50) {
            if (log) { util::println("emplace back"); }

            char val = rand() % 100;
            v.emplace_back(val);
            v_ref.emplace_back(val);
        } else if (act >= 50 && act < 60) {
            if (!v.empty()) {
                if (log) { util::println("pop back"); }

                v.pop_back();
                v_ref.pop_back();
            }
        } else if (act == 60) {
            if (log) { util::println("shrink to fit"); }

            v.shrink_to_fit();
            v_ref.shrink_to_fit();
            VERIFY(v.size() == v.capacity());
        } else if (act == 61) {
            if (log) { util::println("clear"); }

            v.clear();
            v_ref.clear();
        } else if (act == 62) {
            if (log) { util::println("tidy"); }

            v.clear();
            v.shrink_to_fit();
            v_ref.clear();
            v_ref.shrink_to_fit();
            VERIFY(v.size() == 0);
            VERIFY(v.capacity() == 0);
        } else if (act == 63) {
            size_t sz = rand() % 100;

            if (log) {
                if (v.size() < sz) {
                    util::println("resize default grow");
                } else {
                    util::println("resize default trim");
                }
            }

            v.resize(sz);
            v_ref.resize(sz);
        } else if (act == 64) {
            size_t sz = rand() % 100;

            if (log) {
                if (v.size() < sz) {
                    util::println("resize grow");
                } else {
                    util::println("resize trim");
                }
            }

            char val = rand() % 100;
            v.resize(sz, val);
            v_ref.resize(sz, val);
        }

        CHECK(v, v_ref.size(), v_ref.begin());

        if (!v.empty()) {
            VERIFY((v[0] == v_ref[0]) && (v[v.size() - 1] == v_ref[v_ref.size() - 1]));
            VERIFY((v.front() == v_ref.front()) && (v.back() == v_ref.back()));
        }
    }
}

int test_bruteforce() {
#if defined(NDEBUG)
    const int N = 500000000;
#else   // defined(NDEBUG)
    const int N = 5000000;
#endif  // defined(NDEBUG)
    vector_test<util::vector<T>>(N);
    return 0;
}

// --------------------------------------------

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

const int perf_N = 2000000;

int test_perf_T() { return perf<util::vector<T>>(perf_N); }
int test_perf_int() { return perf<util::vector<int>>(4 * perf_N); }
int test_perf_char() { return perf<util::vector<char>>(4 * perf_N); }

int test_perf_T_std() { return perf<std::vector<T>>(perf_N); }
int test_perf_int_std() { return perf<std::vector<int>>(4 * perf_N); }
int test_perf_char_std() { return perf<std::vector<char>>(4 * perf_N); }

// --------------------------------------------

int test_info_sizeof_T() { return sizeof(util::vector<T>); }
int test_info_sizeof_T_iterator() { return sizeof(util::vector<T>::iterator); }

int test_info_sizeof_T_std() { return sizeof(std::vector<T>); }
int test_info_sizeof_T_iterator_std() { return sizeof(std::vector<T>::iterator); }

}  // namespace

ADD_TEST_CASE("1-bruteforce", "vector", test_bruteforce);

ADD_TEST_CASE("2-perf", "vector:T", test_perf_T);
ADD_TEST_CASE("2-perf", "vector:int", test_perf_int);
ADD_TEST_CASE("2-perf", "vector:char", test_perf_char);
ADD_TEST_CASE("2-perf", "<std> vector:T", test_perf_T_std);
ADD_TEST_CASE("2-perf", "<std> vector:int", test_perf_int_std);
ADD_TEST_CASE("2-perf", "<std> vector:char", test_perf_char_std);

ADD_TEST_CASE("3-info", "size: vector:T", test_info_sizeof_T);
ADD_TEST_CASE("3-info", "size: vector:T:iterator", test_info_sizeof_T_iterator);
ADD_TEST_CASE("3-info", "<std> size: vector:T", test_info_sizeof_T_std);
ADD_TEST_CASE("3-info", "<std> size: vector:T:iterator", test_info_sizeof_T_iterator_std);
