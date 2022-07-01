#include "rbtree_tests.h"

#include "uxs/set.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_insert_empty() {
    test_allocator<void> al;

    {
        uxs::set<T, std::less<T>, test_allocator<T>> s(al);
        int tst[] = {1, 2, 3};
        std::initializer_list<Ty> ins;
        std::initializer_list<Ty> ins2 = {1, 2, 2, 3};
        // insert empty to empty
        s.insert(ins);
        CHECK_EMPTY(s);
        VERIFY(T::instance_count == 4);
        VERIFY(al.get_alloc_detected() == 0);
        // insert empty to not empty :
        s.insert(ins2);
        CHECK(s, 3, tst);
        VERIFY(T::instance_count == 7);
        VERIFY(al.get_alloc_detected() == 4);
        VERIFY(al.get_alloc_count() == 3);
        s.insert(ins);
        CHECK(s, 3, tst);
        VERIFY(T::instance_count == 7);
        VERIFY(al.get_alloc_detected() == 4);
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
        int tst[] = {1, 2, 3, 4, 5, 6, 7, 8};
        uxs::set<T, std::less<T>, test_allocator<T>> s(init, al);
        std::initializer_list<Ty> ins = {2, 6, 7, 7, 8};
        s.insert(ins);
        CHECK(s, 8, tst);
        VERIFY(T::instance_count == 18);
        VERIFY(al.get_alloc_detected() == 10);
        VERIFY(al.get_alloc_count() == 8);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "set", test_insert_empty);
ADD_TEST_CASE("", "set", test_insert);
