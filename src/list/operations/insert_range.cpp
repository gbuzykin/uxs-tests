#include "list_tests.h"

#include "uxs/vector.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty, typename Src>
int test_insert_to_empty() {
    test_allocator<void> al;

    {
        int tst[] = {1, 2, 3};
        uxs::list<Ty, test_allocator<Ty>> l(al);
        Src ins = {1, 2, 3};
        //  insert empty
        auto it = l.insert(l.end(), ins.begin(), ins.begin());
        VERIFY(it == l.end());
        CHECK_EMPTY(l);
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 3 : 0));
        VERIFY(al.get_alloc_detected() == 0);
        // not empty
        it = l.insert(l.end(), ins.begin(), ins.end());
        VERIFY(it == l.begin());
        CHECK(l, 3, tst);
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 6 : 3));
        VERIFY(al.get_alloc_detected() == 3);
        VERIFY(al.get_alloc_count() == 3);
        //  insert empty back
        it = l.insert(l.end(), ins.begin(), ins.begin());
        VERIFY(it == l.end());
        CHECK(l, 3, tst);
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 6 : 3));
        VERIFY(al.get_alloc_detected() == 3);
        VERIFY(al.get_alloc_count() == 3);
        //  insert empty mid
        it = l.insert(std::next(l.begin(), 3), ins.begin(), ins.begin());
        VERIFY(it == std::next(l.begin(), 3));
        CHECK(l, 3, tst);
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 6 : 3));
        VERIFY(al.get_alloc_detected() == 3);
        VERIFY(al.get_alloc_count() == 3);
        //  insert empty front
        it = l.insert(l.begin(), ins.begin(), ins.begin());
        VERIFY(it == l.begin());
        CHECK(l, 3, tst);
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 6 : 3));
        VERIFY(al.get_alloc_detected() == 3);
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
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        int tst1[] = {1, 2, 3, 4, 5, 6, 7, 8};
        int tst2[] = {1, 2, 3, 6, 7, 8, 4, 5, 6, 7, 8};
        int tst3[] = {6, 7, 8, 1, 2, 3, 6, 7, 8, 4, 5, 6, 7, 8};
        uxs::list<Ty, test_allocator<Ty>> l(tst_prev, al);
        Src ins = {6, 7, 8};
        // back
        auto it = l.insert(l.end(), ins.begin(), ins.end());
        VERIFY(it == std::prev(l.end(), 3));
        CHECK(l, 8, tst1);
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 16 : 13));
        VERIFY(al.get_alloc_detected() == 8);
        VERIFY(al.get_alloc_count() == 8);
        // mid
        it = l.insert(std::next(l.begin(), 3), ins.begin(), ins.end());
        VERIFY(it == std::next(l.begin(), 3));
        CHECK(l, 11, tst2);
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 19 : 16));
        VERIFY(al.get_alloc_detected() == 11);
        VERIFY(al.get_alloc_count() == 11);
        // front
        it = l.insert(l.begin(), ins.begin(), ins.end());
        VERIFY(it == l.begin());
        CHECK(l, 14, tst3);
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 22 : 19));
        VERIFY(al.get_alloc_detected() == 14);
        VERIFY(al.get_alloc_count() == 14);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_insert_to_empty_assignable() { return test_insert_to_empty<T, uxs::vector<T>>(); }
int test_insert_assignable() { return test_insert<T, uxs::vector<T>>(); }
int test_insert_to_empty_not_assignable() { return test_insert_to_empty<T, uxs::vector<int>>(); }
int test_insert_not_assignable() { return test_insert<T, uxs::vector<int>>(); }

}  // namespace

ADD_TEST_CASE("", "list", test_insert_to_empty_assignable);
ADD_TEST_CASE("", "list", test_insert_assignable);
ADD_TEST_CASE("", "list", test_insert_to_empty_not_assignable);
ADD_TEST_CASE("", "list", test_insert_not_assignable);
