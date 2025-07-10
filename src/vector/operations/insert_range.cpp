#include "vector_tests.h"

#include <uxs-legacy/list.h>

using namespace uxs_test_suite;

namespace {

template<typename Ty, typename Src>
int test_insert_empty() {
    test_allocator<void> al;

    {
        int tst[] = {1, 2, 3};
        uxs::vector<Ty, test_allocator<Ty>> v(al);
        Src ins = {1, 2, 3};
        // insert empty
        auto it = v.insert(v.end(), ins.begin(), ins.begin());
        VERIFY(it == v.end());
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 3 : 0));
        VERIFY(al.get_alloc_detected() == 0);
        // insert empty to not empty :
        it = v.insert(v.end(), ins.begin(), ins.end());
        VERIFY(it == v.begin());
        CHECK(v, 3, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 6 : 3));
        VERIFY(al.get_alloc_detected() == v.capacity());
        // back
        it = v.insert(v.end(), ins.begin(), ins.begin());
        VERIFY(it == v.end());
        CHECK(v, 3, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 6 : 3));
        VERIFY(al.get_alloc_detected() == v.capacity());
        // mid
        it = v.insert(v.begin() + 1, ins.begin(), ins.begin());
        VERIFY(it == v.begin() + 1);
        CHECK(v, 3, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 6 : 3));
        VERIFY(al.get_alloc_detected() == v.capacity());
        // front
        it = v.insert(v.begin(), ins.begin(), ins.begin());
        VERIFY(it == v.begin());
        CHECK(v, 3, tst);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 6 : 3));
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty, typename Src>
int test_insert_no_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        int tst1[] = {1, 2, 3, 4, 5, 6, 7, 8};
        int tst2[] = {1, 2, 3, 6, 7, 8, 4, 5, 6, 7, 8};
        int tst3[] = {6, 7, 8, 1, 2, 3, 6, 7, 8, 4, 5, 6, 7, 8};
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
        Src ins = {6, 7, 8};
        v.reserve(20);
        al.reset_alloc_detected();
        // back
        auto it = v.insert(v.end(), ins.begin(), ins.end());
        VERIFY(it == v.end() - 3);
        CHECK(v, 8, tst1);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 16 : 13));
        VERIFY(al.get_alloc_detected() == 0);
        // mid
        it = v.insert(v.begin() + 3, ins.begin(), ins.end());
        VERIFY(it == v.begin() + 3);
        CHECK(v, 11, tst2);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 19 : 16));
        VERIFY(al.get_alloc_detected() == 0);
        // front
        it = v.insert(v.begin(), ins.begin(), ins.end());
        VERIFY(it == v.begin());
        CHECK(v, 14, tst3);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 22 : 19));
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty, typename Src>
int test_insert_needs_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        int tst1[] = {1, 2, 3, 4, 5, 6, 7, 8};
        int tst2[] = {1, 2, 3, 6, 7, 8, 4, 5, 6, 7, 8};
        int tst3[] = {6, 7, 8, 1, 2, 3, 6, 7, 8, 4, 5, 6, 7, 8};
        uxs::vector<Ty, test_allocator<Ty>> v(init, al);
        Src ins = {6, 7, 8};
        // back
        v.shrink_to_fit();
        al.reset_alloc_detected();
        auto it = v.insert(v.end(), ins.begin(), ins.end());
        VERIFY(it == v.end() - 3);
        CHECK(v, 8, tst1);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 16 : 13));
        VERIFY(al.get_alloc_detected() == v.capacity());
        // mid
        v.shrink_to_fit();
        al.reset_alloc_detected();
        it = v.insert(v.begin() + 3, ins.begin(), ins.end());
        VERIFY(it == v.begin() + 3);
        CHECK(v, 11, tst2);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 19 : 16));
        VERIFY(al.get_alloc_detected() == v.capacity());
        // front
        v.shrink_to_fit();
        al.reset_alloc_detected();
        it = v.insert(v.begin(), ins.begin(), ins.end());
        VERIFY(it == v.begin());
        CHECK(v, 14, tst3);
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 22 : 19));
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_insert_empty_random_access_range_assignable() { return test_insert_empty<T, uxs::vector<T>>(); }
int test_insert_no_realloc_random_access_range_assignable() { return test_insert_no_realloc<T, uxs::vector<T>>(); }
int test_insert_needs_realloc_random_access_range_assignable_nothrow_move() {
    return test_insert_needs_realloc<T, uxs::vector<T>>();
}
int test_insert_needs_realloc_random_access_range_assignable_throwing_move() {
    return test_insert_needs_realloc<T_ThrowingMove, uxs::vector<T_ThrowingMove>>();
}

int test_insert_empty_generic_input_range_assignable() { return test_insert_empty<T, uxs::list<T>>(); }
int test_insert_no_realloc_generic_input_range_assignable() { return test_insert_no_realloc<T, uxs::list<T>>(); }
int test_insert_needs_realloc_generic_input_range_assignable_nothrow_move() {
    return test_insert_needs_realloc<T, uxs::list<T>>();
}
int test_insert_needs_realloc_generic_input_range_assignable_throwing_move() {
    return test_insert_needs_realloc<T_ThrowingMove, uxs::list<T_ThrowingMove>>();
}

int test_insert_empty_random_access_range_not_assignable() { return test_insert_empty<T, uxs::vector<int>>(); }
int test_insert_no_realloc_random_access_range_not_assignable() {
    return test_insert_no_realloc<T, uxs::vector<int>>();
}
int test_insert_needs_realloc_random_access_range_not_assignable_nothrow_move() {
    return test_insert_needs_realloc<T, uxs::vector<int>>();
}
int test_insert_needs_realloc_random_access_range_not_assignable_throwing_move() {
    return test_insert_needs_realloc<T_ThrowingMove, uxs::vector<int>>();
}

int test_insert_empty_generic_input_range_not_assignable() { return test_insert_empty<T, uxs::list<int>>(); }
int test_insert_no_realloc_generic_input_range_not_assignable() { return test_insert_no_realloc<T, uxs::list<int>>(); }
int test_insert_needs_realloc_generic_input_range_not_assignable_nothrow_move() {
    return test_insert_needs_realloc<T, uxs::list<int>>();
}
int test_insert_needs_realloc_generic_input_range_not_assignable_throwing_move() {
    return test_insert_needs_realloc<T_ThrowingMove, uxs::list<int>>();
}

}  // namespace

ADD_TEST_CASE("", "vector", test_insert_empty_random_access_range_assignable);
ADD_TEST_CASE("", "vector", test_insert_no_realloc_random_access_range_assignable);
ADD_TEST_CASE("", "vector", test_insert_needs_realloc_random_access_range_assignable_nothrow_move);
ADD_TEST_CASE("", "vector", test_insert_needs_realloc_random_access_range_assignable_throwing_move);

ADD_TEST_CASE("", "vector", test_insert_empty_generic_input_range_assignable);
ADD_TEST_CASE("", "vector", test_insert_no_realloc_generic_input_range_assignable);
ADD_TEST_CASE("", "vector", test_insert_needs_realloc_generic_input_range_assignable_nothrow_move);
ADD_TEST_CASE("", "vector", test_insert_needs_realloc_generic_input_range_assignable_throwing_move);

ADD_TEST_CASE("", "vector", test_insert_empty_random_access_range_not_assignable);
ADD_TEST_CASE("", "vector", test_insert_no_realloc_random_access_range_not_assignable);
ADD_TEST_CASE("", "vector", test_insert_needs_realloc_random_access_range_not_assignable_nothrow_move);
ADD_TEST_CASE("", "vector", test_insert_needs_realloc_random_access_range_not_assignable_throwing_move);

ADD_TEST_CASE("", "vector", test_insert_empty_generic_input_range_not_assignable);
ADD_TEST_CASE("", "vector", test_insert_no_realloc_generic_input_range_not_assignable);
ADD_TEST_CASE("", "vector", test_insert_needs_realloc_generic_input_range_not_assignable_nothrow_move);
ADD_TEST_CASE("", "vector", test_insert_needs_realloc_generic_input_range_not_assignable_throwing_move);
