#include "rbtree_tests.h"

#include <uxs-legacy/set.h>

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_emplace_to_empty() {
    test_allocator<void> al;

    {
        int tst[] = {10};
        uxs::set<T, std::less<T>, test_allocator<T>> s(al);
        auto result = s.emplace(10);
        VERIFY(result.second && result.first == s.begin());
        CHECK(s, 1, tst);
        VERIFY(T::instance_count == 1);
        VERIFY(T::not_empty_count == 1);
        VERIFY(al.get_alloc_detected() == 1);
        VERIFY(al.get_alloc_count() == 1);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_emplace() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {2, 3, 4, 6, 7};
        int tst1[] = {2, 3, 4, 6, 7, 8};
        int tst2[] = {2, 3, 4, 5, 6, 7, 8};
        int tst3[] = {1, 2, 3, 4, 5, 6, 7, 8};
        uxs::set<T, std::less<T>, test_allocator<T>> s(init, al);
        // back
        auto result = s.emplace(8);
        VERIFY(result.second && result.first == std::prev(s.end()));
        CHECK(s, 6, tst1);
        VERIFY(T::instance_count == 11);
        VERIFY(al.get_alloc_detected() == 6);
        VERIFY(al.get_alloc_count() == 6);
        // mid
        result = s.emplace(5);
        VERIFY(result.second && result.first == std::next(s.begin(), 3));
        CHECK(s, 7, tst2);
        VERIFY(T::instance_count == 12);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 7);
        // front
        result = s.emplace(1);
        VERIFY(result.second && result.first == s.begin());
        CHECK(s, 8, tst3);
        VERIFY(T::instance_count == 13);
        VERIFY(al.get_alloc_detected() == 8);
        VERIFY(al.get_alloc_count() == 8);
        // existing
        result = s.emplace(1);
        VERIFY(!result.second && result.first == s.begin());
        CHECK(s, 8, tst3);
        VERIFY(T::instance_count == 13);
        VERIFY(al.get_alloc_detected() == 9);
        VERIFY(al.get_alloc_count() == 8);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "set", test_emplace_to_empty);
ADD_TEST_CASE("", "set", test_emplace);
