#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_emplace_to_empty() {
    test_allocator<void> al;

    {
        int tst[] = {10};
        uxs::list<Ty, test_allocator<Ty>> l(al);
        auto it = l.emplace(l.end(), 10);
        VERIFY(it == l.begin());
        CHECK(l, 1, tst);
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
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        int tst1[] = {1, 2, 3, 4, 5, 6};
        int tst2[] = {1, 2, 3, 7, 4, 5, 6};
        int tst3[] = {8, 1, 2, 3, 7, 4, 5, 6};
        uxs::list<Ty, test_allocator<Ty>> l(tst_prev, al);
        // back
        auto it = l.emplace(l.end(), 6);
        VERIFY(it == std::prev(l.end()));
        CHECK(l, 6, tst1);
        VERIFY(T::instance_count == 11);
        VERIFY(al.get_alloc_detected() == 6);
        VERIFY(al.get_alloc_count() == 6);
        // mid
        it = l.emplace(std::next(l.begin(), 3), 7);
        VERIFY(it == std::next(l.begin(), 3));
        CHECK(l, 7, tst2);
        VERIFY(T::instance_count == 12);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 7);
        // front
        it = l.emplace(l.begin(), 8);
        VERIFY(it == l.begin());
        CHECK(l, 8, tst3);
        VERIFY(T::instance_count == 13);
        VERIFY(al.get_alloc_detected() == 8);
        VERIFY(al.get_alloc_count() == 8);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_emplace_to_empty);
ADD_TEST_CASE("", "list", test_emplace);
