#include "vector_tests.h"

#include <vector>

using namespace uxs_test_suite;

namespace {

template<typename VecType>
void vector_test(int iter_count, bool log = false) {
    using value_type = typename VecType::value_type;

    VecType v;
    std::vector<value_type> v_ref;

    srand(0);

    if (log) { uxs::stdbuf::out.endl(); }

    for (int iter = 0, perc0 = -1; iter < iter_count; ++iter) {
        if (!log) {
            int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
            if (perc > perc0) {
                uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
                perc0 = perc;
            }
        }

        int act = rand() % 65;
        if (act >= 0 && act < 10) {
            if (log) { uxs::println("insert one"); }

            size_t n = rand() % (v.size() + 1);
            char val = rand() % 100;
            auto t_res = v.emplace(v.begin() + n, val);
            auto ref_res = v_ref.emplace(v_ref.begin() + n, val);
            VERIFY(std::distance(v.begin(), t_res) == std::distance(v_ref.begin(), ref_res));
        } else if (act >= 10 && act < 20) {
            if (log) { uxs::println("insert"); }

            size_t n = rand() % (v.size() + 1);
            size_t count = 1 + rand() % 5;

            value_type val[10];
            for (size_t i = 0; i < count; ++i) { val[i] = T(rand() % 100); }

            auto t_res = v.insert(v.begin() + n, val, val + count);
            auto ref_res = v_ref.insert(v_ref.begin() + n, val, val + count);
            VERIFY(std::distance(v.begin(), t_res) == std::distance(v_ref.begin(), ref_res));
        } else if (act >= 20 && act < 30) {
            if (!v.empty()) {
                if (log) { uxs::println("erase one"); }

                size_t n = rand() % v.size();
                auto t_res = v.erase(v.begin() + n);
                auto ref_res = v_ref.erase(v_ref.begin() + n);
                VERIFY(std::distance(v.begin(), t_res) == std::distance(v_ref.begin(), ref_res));
            }
        } else if (act >= 30 && act < 40) {
            if (!v.empty()) {
                if (log) { uxs::println("erase"); }

                size_t n = rand() % (v.size() + 1);
                size_t count = rand() % (v.size() + 1 - n);
                auto t_res = v.erase(v.begin() + n, v.begin() + n + count);
                auto ref_res = v_ref.erase(v_ref.begin() + n, v_ref.begin() + n + count);
                VERIFY(std::distance(v.begin(), t_res) == std::distance(v_ref.begin(), ref_res));
            }
        } else if (act >= 40 && act < 50) {
            if (log) { uxs::println("emplace back"); }

            char val = rand() % 100;
            v.emplace_back(val);
            v_ref.emplace_back(val);
        } else if (act >= 50 && act < 60) {
            if (!v.empty()) {
                if (log) { uxs::println("pop back"); }

                v.pop_back();
                v_ref.pop_back();
            }
        } else if (act == 60) {
            if (log) { uxs::println("shrink to fit"); }

            v.shrink_to_fit();
            v_ref.shrink_to_fit();
            VERIFY(v.size() == v.capacity());
        } else if (act == 61) {
            if (log) { uxs::println("clear"); }

            v.clear();
            v_ref.clear();
        } else if (act == 62) {
            if (log) { uxs::println("tidy"); }

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
                    uxs::println("resize default grow");
                } else {
                    uxs::println("resize default trim");
                }
            }

            v.resize(sz);
            v_ref.resize(sz);
        } else if (act == 64) {
            size_t sz = rand() % 100;

            if (log) {
                if (v.size() < sz) {
                    uxs::println("resize grow");
                } else {
                    uxs::println("resize trim");
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
    vector_test<uxs::vector<T>>(N);
    return 0;
}

}  // namespace

ADD_TEST_CASE("1-bruteforce", "vector", test_bruteforce);
