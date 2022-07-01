#include "rbtree_tests.h"

#include "uxs/multiset.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_insert_empty() {
    test_allocator<void> al;

    {
        uxs::multiset<T, std::less<T>, test_allocator<T>> ms(al);
        std::initializer_list<Ty> ins;
        std::initializer_list<Ty> ins2 = {1, 2, 3};
        // insert empty to empty
        ms.insert(ins);
        CHECK_EMPTY(ms);
        VERIFY(T::instance_count == 3);
        VERIFY(al.get_alloc_detected() == 0);
        // insert empty to not empty :
        ms.insert(ins2);
        CHECK(ms, ins2.size(), ins2.begin());
        VERIFY(T::instance_count == 6);
        VERIFY(al.get_alloc_detected() == 3);
        VERIFY(al.get_alloc_count() == 3);
        ms.insert(ins);
        CHECK(ms, ins2.size(), ins2.begin());
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
        int tst[] = {1, 2, 3, 4, 5, 6, 7, 8};
        uxs::multiset<T, std::less<T>, test_allocator<T>> ms(init, al);
        std::initializer_list<Ty> ins = {6, 7, 8};
        ms.insert(ins);
        CHECK(ms, 8, tst);
        VERIFY(T::instance_count == 16);
        VERIFY(al.get_alloc_detected() == 8);
        VERIFY(al.get_alloc_count() == 8);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "multiset", test_insert_empty);
ADD_TEST_CASE("", "multiset", test_insert);
