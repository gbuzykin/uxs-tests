#include "rbtree_tests.h"

#include <uxs-legacy/multiset.h>
#include <uxs-legacy/set.h>

#include <random>
#include <set>

namespace uxs_test_suite {

template<typename SetType>
void rbtree_bruteforce_test(int iter_count, bool log = false) {
    using value_type = typename SetType::value_type;
    using key_compare = typename SetType::key_compare;
    using allocator_type = typename SetType::allocator_type;

    using RefSetType =
        typename std::conditional<std::is_same<SetType, uxs::set<value_type, key_compare, allocator_type>>::value,
                                  std::set<value_type>, std::multiset<value_type>>::type;

    std::default_random_engine gen;
    std::uniform_int_distribution<int> d(0, 499);

    for (int iter = 0, perc0 = -1; iter < iter_count; ++iter) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
            perc0 = perc;
        }

        SetType s, s1, s2, s3;
        RefSetType s_ref;

        for (size_t i = 0; i < 100; ++i) { s1.emplace(d(gen)); }
        for (size_t i = 0; i < 1100; ++i) { s2.emplace(d(gen)); }
        for (size_t i = 0; i < 1000; ++i) {
            int val = d(gen);
            s3.emplace(val);
            s_ref.emplace(val);
            CHECK(s3, s_ref.size(), s_ref.begin());
        }

        s = s1;
        s = s2;
        s = s3;
        CHECK(s, s_ref.size(), s_ref.begin());

        s = decltype(s)();
        CHECK_EMPTY(s);

        s = s3;

        for (size_t i = 0; i < 250; ++i) {
            int val = d(gen);

            auto it = s.find(val);
            auto lower = s.lower_bound(val);
            auto upper = s.upper_bound(val);
            auto range = s.equal_range(val);

            VERIFY(lower == range.first);
            VERIFY(upper == range.second);

            auto it_ref = s_ref.find(val);
            auto lower_ref = s_ref.lower_bound(val);
            auto upper_ref = s_ref.upper_bound(val);

            VERIFY(s.count(val) == s_ref.count(val));

            VERIFY(std::distance(s.begin(), it) == std::distance(s_ref.begin(), it_ref));
            VERIFY(std::distance(s.begin(), lower) == std::distance(s_ref.begin(), lower_ref));
            VERIFY(std::distance(s.begin(), upper) == std::distance(s_ref.begin(), upper_ref));

            if ((lower != s.end()) && (lower == upper)) {
                it = s.erase(lower);
            } else {
                it = s.erase(lower, upper);
            }

            if ((lower_ref != s_ref.end()) && (lower_ref == upper_ref)) {
                it_ref = s_ref.erase(lower_ref);
            } else {
                it_ref = s_ref.erase(lower_ref, upper_ref);
            }

            VERIFY(std::distance(s.begin(), it) == std::distance(s_ref.begin(), it_ref));
            CHECK(s, s_ref.size(), s_ref.begin());
        }

        VERIFY(s.empty() == s_ref.empty());

        for (size_t i = 0; i < 500; ++i) {
            int val = d(gen);
            VERIFY(s.erase(val) == s_ref.erase(val));
            CHECK(s, s_ref.size(), s_ref.begin());
        }

        VERIFY(s.empty() == s_ref.empty());

        while (!s_ref.empty()) {
            int val = d(gen);
            auto it = s.lower_bound(val);
            if (it != s.end()) { it = s.erase(it); }

            auto it_ref = s_ref.lower_bound(val);
            if (it_ref != s_ref.end()) { it_ref = s_ref.erase(it_ref); }

            VERIFY(std::distance(s.begin(), it) == std::distance(s_ref.begin(), it_ref));
            CHECK(s, s_ref.size(), s_ref.begin());
        }
    }
}

}  // namespace uxs_test_suite
