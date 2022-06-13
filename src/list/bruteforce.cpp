#include "list_tests.h"

#include "uxs/pool_allocator.h"

#include <list>

using namespace uxs_test_suite;

namespace uxs {
template class list<T>;
template class list<T_ThrowingMove>;
}  // namespace uxs

namespace {

// --------------------------------------------

template<typename ListType>
void list_test(int iter_count, bool log = false) {
    using value_type = typename ListType::value_type;

    ListType l;
    std::list<value_type> l_ref;

    srand(0);

    if (log) { uxs::stdbuf::out.endl(); }

    for (int iter = 0, perc0 = -1; iter < iter_count; ++iter) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
            perc0 = perc;
        }

        int act = rand() % 83;
        if (act >= 0 && act < 10) {
            if (log) { uxs::println("insert one"); }

            size_t n = rand() % (l.size() + 1);
            int val = rand() % 100;
            auto l_res = l.emplace(std::next(l.begin(), n), val);
            auto ref_res = l_ref.emplace(std::next(l_ref.begin(), n), val);
            VERIFY(std::distance(l.begin(), l_res) == std::distance(l_ref.begin(), ref_res));
        } else if (act >= 10 && act < 20) {
            if (log) { uxs::println("insert"); }

            size_t n = rand() % (l.size() + 1);
            size_t count = 1 + rand() % 5;

            T val[10];
            for (size_t i = 0; i < count; ++i) { val[i] = T(rand() % 100); }

            auto l_res = l.insert(std::next(l.begin(), n), val, val + count);
            auto ref_res = l_ref.insert(std::next(l_ref.begin(), n), val, val + count);
            VERIFY(std::distance(l.begin(), l_res) == std::distance(l_ref.begin(), ref_res));
        } else if (act >= 20 && act < 30) {
            if (!l.empty()) {
                if (log) { uxs::println("erase one"); }

                size_t n = rand() % l.size();
                auto l_res = l.erase(std::next(l.begin(), n));
                auto ref_res = l_ref.erase(std::next(l_ref.begin(), n));
                VERIFY(std::distance(l.begin(), l_res) == std::distance(l_ref.begin(), ref_res));
            }
        } else if (act >= 30 && act < 40) {
            if (!l.empty()) {
                if (log) { uxs::println("erase"); }

                size_t n = rand() % (1 + l.size());
                size_t count = rand() % (1 + l.size() - n);
                auto l_res = l.erase(std::next(l.begin(), n), std::next(l.begin(), n + count));
                auto ref_res = l_ref.erase(std::next(l_ref.begin(), n), std::next(l_ref.begin(), n + count));
                VERIFY(std::distance(l.begin(), l_res) == std::distance(l_ref.begin(), ref_res));
            }
        } else if (act >= 40 && act < 50) {
            if (log) { uxs::println("emplace back"); }

            int val = rand() % 100;
            l.emplace_back(val);
            l_ref.emplace_back(val);
        } else if (act >= 50 && act < 60) {
            if (!l.empty()) {
                if (log) { uxs::println("pop back"); }

                l.pop_back();
                l_ref.pop_back();
            }
        } else if (act >= 60 && act < 70) {
            if (log) { uxs::println("emplace front"); }

            int val = rand() % 100;
            l.emplace_front(val);
            l_ref.emplace_front(val);
        } else if (act >= 70 && act < 80) {
            if (!l.empty()) {
                if (log) { uxs::println("pop front"); }

                l.pop_front();
                l_ref.pop_front();
            }
        } else if (act == 80) {
            if (log) { uxs::println("clear"); }

            l.clear();
            l_ref.clear();
        } else if (act == 81) {
            size_t sz = rand() % 100;

            if (log) {
                if (l.size() < sz) {
                    uxs::println("resize default grow");
                } else {
                    uxs::println("resize default trim");
                }
            }

            l.resize(sz);
            l_ref.resize(sz);
        } else if (act == 82) {
            size_t sz = rand() % 100;

            if (log) {
                if (l.size() < sz) {
                    uxs::println("resize grow");
                } else {
                    uxs::println("resize trim");
                }
            }

            int val = rand() % 100;
            l.resize(sz, val);
            l_ref.resize(sz, val);
        }

        CHECK(l, l_ref.size(), l_ref.begin());
        if (!l.empty()) { VERIFY((l.front() == l_ref.front()) && (l.back() == l_ref.back())); }
    }
}

void list_sort_test(int iter_count) {
    test_allocator<void> al;
    uxs::list<T, test_allocator<T>> l{al};

    srand(0);

    for (int iter = 0, perc0 = -1; iter < iter_count; ++iter) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
            perc0 = perc;
        }

        l.clear();
        for (size_t i = 0; i < 100000; ++i) { l.emplace_back(rand() % 100000); }

        l.sort();

        size_t i = 1;
        for (auto it = std::next(l.begin()); it != l.end(); ++it, ++i) { VERIFY(!(*it < *std::prev(it))); }
        VERIFY(i == 100000);
    }
}

int test_bruteforce1() {
#if defined(NDEBUG)
    const int N = 500000000;
#else   // defined(NDEBUG)
    const int N = 5000000;
#endif  // defined(NDEBUG)
    list_test<uxs::list<T, uxs::global_pool_allocator<T>>>(N);
    return 0;
}

int test_bruteforce2() {
#if defined(NDEBUG)
    const int N = 5000;
#else   // defined(NDEBUG)
    const int N = 50;
#endif  // defined(NDEBUG)
    list_sort_test(N);
    return 0;
}

}  // namespace

ADD_TEST_CASE("1-bruteforce", "list", test_bruteforce1);
ADD_TEST_CASE("1-bruteforce", "list", test_bruteforce2);
