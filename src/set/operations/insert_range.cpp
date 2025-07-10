#include "rbtree_tests.h"

#include <uxs-legacy/set.h>
#include <uxs-legacy/vector.h>

using namespace uxs_test_suite;

namespace {

template<typename Ty, typename Src>
int test_insert_empty() {
    test_allocator<void> al;

    {
        uxs::set<T, std::less<T>, test_allocator<T>> s(al);
        Src ins = {1, 1, 2, 3, 3};
        int tst[] = {1, 2, 3};
        // insert empty
        s.insert(ins.begin(), ins.begin());
        CHECK_EMPTY(s);
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 5 : 0));
        VERIFY(al.get_alloc_detected() == 0);
        // insert empty to not empty :
        s.insert(ins.begin(), ins.end());
        CHECK(s, 3, tst);
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 8 : 3));
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 3);
        s.insert(ins.begin(), ins.begin());
        CHECK(s, 3, tst);
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 8 : 3));
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 3);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty, typename Src>
int test_insert() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        int tst[] = {1, 2, 3, 4, 5, 6, 7, 8};
        uxs::set<T, std::less<T>, test_allocator<T>> s(init, al);
        Src ins = {6, 6, 7, 8, 8};
        s.insert(ins.begin(), ins.end());
        CHECK(s, 8, tst);
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 18 : 13));
        VERIFY(al.get_alloc_detected() == 10);
        VERIFY(al.get_alloc_count() == 8);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_insert_empty_assignable() { return test_insert_empty<T, uxs::vector<T>>(); }
int test_insert_assignable() { return test_insert<T, uxs::vector<T>>(); }
int test_insert_empty_not_assignable() { return test_insert_empty<T, uxs::vector<int>>(); }
int test_insert_not_assignable() { return test_insert<T, uxs::vector<int>>(); }

}  // namespace

ADD_TEST_CASE("", "set", test_insert_empty_assignable);
ADD_TEST_CASE("", "set", test_insert_assignable);
ADD_TEST_CASE("", "set", test_insert_empty_not_assignable);
ADD_TEST_CASE("", "set", test_insert_not_assignable);
