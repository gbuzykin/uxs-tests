#include "vector_tests.h"

#include "uxs/list.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty, typename Src>
int test_range_empty_to_empty() {
    test_allocator<void> al;

    {
        Src tst;
        uxs::vector<Ty, test_allocator<Ty>> v(al);
        v.assign(tst.begin(), tst.end());
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty, typename Src>
int test_range_not_empty_to_empty() {
    test_allocator<void> al;

    {
        Src tst = {1, 2, 3, 4, 5};
        uxs::vector<Ty, test_allocator<Ty>> v(al);
        v.assign(tst.begin(), tst.end());
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty, typename Src>
int test_range_more_no_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        Src tst = {11, 12, 13, 14, 15, 16, 17};
        uxs::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.reserve(10);
        al.reset_alloc_detected();
        v.assign(tst.begin(), tst.end());
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 19);
        VERIFY(T::not_empty_count == 19);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty, typename Src>
int test_range_more_needs_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        Src tst = {11, 12, 13, 14, 15, 16, 17};
        uxs::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.shrink_to_fit();
        al.reset_alloc_detected();
        v.assign(tst.begin(), tst.end());
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 19);
        VERIFY(T::not_empty_count == 19);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty, typename Src>
int test_range_less() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5, 6, 7};
        Src tst = {11, 12, 13, 14, 15};
        uxs::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.assign(tst.begin(), tst.end());
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 17);
        VERIFY(T::not_empty_count == 17);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty, typename Src>
int test_range_same_amount() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        Src tst = {11, 12, 13, 14, 15};
        uxs::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.assign(tst.begin(), tst.end());
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(T::instance_count == 15);
        VERIFY(T::not_empty_count == 15);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty, typename Src>
int test_range_empty_to_not_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        Src tst;
        uxs::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.assign(tst.begin(), tst.end());
        CHECK_EMPTY(v);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_range_empty_to_empty_random_access_range() { return test_range_empty_to_empty<T, uxs::vector<T>>(); }
int test_range_not_empty_to_empty_random_access_range() { return test_range_not_empty_to_empty<T, uxs::vector<T>>(); }
int test_range_more_no_realloc_random_access_range() { return test_range_more_no_realloc<T, uxs::vector<T>>(); }
int test_range_more_needs_realloc_random_access_range_nothrow_move() {
    return test_range_more_needs_realloc<T, uxs::vector<T>>();
}
int test_range_more_needs_realloc_random_access_range_throwing_move() {
    return test_range_more_needs_realloc<T_ThrowingMove, uxs::vector<T_ThrowingMove>>();
}
int test_range_less_random_access_range() { return test_range_less<T, uxs::vector<T>>(); }
int test_range_same_amount_random_access_range() { return test_range_same_amount<T, uxs::vector<T>>(); }
int test_range_empty_to_not_empty_random_access_range() { return test_range_empty_to_not_empty<T, uxs::vector<T>>(); }

int test_range_empty_to_empty_generic_input_range() { return test_range_empty_to_empty<T, uxs::list<T>>(); }
int test_range_not_empty_to_empty_generic_input_range() { return test_range_not_empty_to_empty<T, uxs::list<T>>(); }
int test_range_more_no_realloc_generic_input_range() { return test_range_more_no_realloc<T, uxs::list<T>>(); }
int test_range_more_needs_realloc_generic_input_range_nothrow_move() {
    return test_range_more_needs_realloc<T, uxs::list<T>>();
}
int test_range_more_needs_realloc_generic_input_range_throwing_move() {
    return test_range_more_needs_realloc<T_ThrowingMove, uxs::list<T_ThrowingMove>>();
}
int test_range_less_generic_input_range() { return test_range_less<T, uxs::list<T>>(); }
int test_range_same_amount_generic_input_range() { return test_range_same_amount<T, uxs::list<T>>(); }
int test_range_empty_to_not_empty_generic_input_range() { return test_range_empty_to_not_empty<T, uxs::list<T>>(); }

}  // namespace

ADD_TEST_CASE("", "vector", test_range_empty_to_empty_random_access_range);
ADD_TEST_CASE("", "vector", test_range_not_empty_to_empty_random_access_range);
ADD_TEST_CASE("", "vector", test_range_more_no_realloc_random_access_range);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_random_access_range_nothrow_move);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_random_access_range_throwing_move);
ADD_TEST_CASE("", "vector", test_range_less_random_access_range);
ADD_TEST_CASE("", "vector", test_range_same_amount_random_access_range);
ADD_TEST_CASE("", "vector", test_range_empty_to_not_empty_random_access_range);

ADD_TEST_CASE("", "vector", test_range_empty_to_empty_generic_input_range);
ADD_TEST_CASE("", "vector", test_range_not_empty_to_empty_generic_input_range);
ADD_TEST_CASE("", "vector", test_range_more_no_realloc_generic_input_range);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_generic_input_range_nothrow_move);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_generic_input_range_throwing_move);
ADD_TEST_CASE("", "vector", test_range_less_generic_input_range);
ADD_TEST_CASE("", "vector", test_range_same_amount_generic_input_range);
ADD_TEST_CASE("", "vector", test_range_empty_to_not_empty_generic_input_range);
