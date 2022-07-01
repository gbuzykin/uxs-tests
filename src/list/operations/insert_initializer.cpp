#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_insert_empty() {
    test_allocator<void> al;

    {
        uxs::list<Ty, test_allocator<Ty>> l(al);
        std::initializer_list<Ty> ins;
        std::initializer_list<Ty> ins2 = {1, 2, 3};
        // insert empty to empty
        auto it = l.insert(l.end(), ins);
        VERIFY(it == l.end());
        CHECK_EMPTY(l);
        VERIFY(T::instance_count == 3);
        VERIFY(al.get_alloc_detected() == 0);
        // insert empty to not empty :
        it = l.insert(l.end(), ins2);
        VERIFY(it == l.begin());
        CHECK(l, ins2.size(), ins2.begin());
        VERIFY(T::instance_count == 6);
        VERIFY(al.get_alloc_detected() == 3);
        VERIFY(al.get_alloc_count() == 3);
        // back
        it = l.insert(l.end(), ins);
        VERIFY(it == l.end());
        CHECK(l, ins2.size(), ins2.begin());
        VERIFY(T::instance_count == 6);
        VERIFY(al.get_alloc_detected() == 3);
        VERIFY(al.get_alloc_count() == 3);
        // mid
        it = l.insert(std::next(l.begin(), 1), ins);
        VERIFY(it == std::next(l.begin(), 1));
        CHECK(l, ins2.size(), ins2.begin());
        VERIFY(T::instance_count == 6);
        VERIFY(al.get_alloc_detected() == 3);
        VERIFY(al.get_alloc_count() == 3);
        // front
        it = l.insert(l.begin(), ins);
        VERIFY(it == l.begin());
        CHECK(l, ins2.size(), ins2.begin());
        VERIFY(T::instance_count == 6);
        VERIFY(al.get_alloc_detected() == 3);
        VERIFY(al.get_alloc_count() == 3);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_insert() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        int tst1[] = {1, 2, 3, 4, 5, 6, 7, 8};
        int tst2[] = {1, 2, 3, 6, 7, 8, 4, 5, 6, 7, 8};
        int tst3[] = {6, 7, 8, 1, 2, 3, 6, 7, 8, 4, 5, 6, 7, 8};
        uxs::list<Ty, test_allocator<Ty>> l(init, al);
        std::initializer_list<Ty> ins = {6, 7, 8};
        // back
        auto it = l.insert(l.end(), ins);
        VERIFY(it == std::prev(l.end(), 3));
        CHECK(l, 8, tst1);
        VERIFY(T::instance_count == 16);
        VERIFY(al.get_alloc_detected() == 8);
        VERIFY(al.get_alloc_count() == 8);
        // mid
        it = l.insert(std::next(l.begin(), 3), ins);
        VERIFY(it == std::next(l.begin(), 3));
        CHECK(l, 11, tst2);
        VERIFY(T::instance_count == 19);
        VERIFY(al.get_alloc_detected() == 11);
        VERIFY(al.get_alloc_count() == 11);
        // front
        it = l.insert(l.begin(), ins);
        VERIFY(it == l.begin());
        CHECK(l, 14, tst3);
        VERIFY(T::instance_count == 22);
        VERIFY(al.get_alloc_detected() == 14);
        VERIFY(al.get_alloc_count() == 14);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_insert_empty);
ADD_TEST_CASE("", "list", test_insert);
