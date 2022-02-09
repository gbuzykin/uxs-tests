#include "vector_tests.h"

#include <list>
#include <vector>

using namespace util_test_suite;

namespace {

template<typename Ty, typename Src>
int test_range_empty_to_empty() {
    test_allocator<void> al;

    {
        Src tst;
        util::vector<Ty, test_allocator<Ty>> v(al);
        v.assign(tst.begin(), tst.end());
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(Ty::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty, typename Src>
int test_range_not_empty_to_empty() {
    test_allocator<void> al;

    {
        Src tst = {1, 2, 3, 4, 5};
        util::vector<Ty, test_allocator<Ty>> v(al);
        v.assign(tst.begin(), tst.end());
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(Ty::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 10 : 5));
        VERIFY(Ty::not_empty_count == (std::is_same<typename Src::value_type, Ty>::value ? 10 : 5));
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty, typename Src>
int test_range_more_no_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        Src tst = {11, 12, 13, 14, 15, 16, 17};
        util::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.reserve(10);
        al.reset_alloc_detected();
        v.assign(tst.begin(), tst.end());
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(Ty::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 19 : 12));
        VERIFY(Ty::not_empty_count == (std::is_same<typename Src::value_type, Ty>::value ? 19 : 12));
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty, typename Src>
int test_range_more_needs_realloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        Src tst = {11, 12, 13, 14, 15, 16, 17};
        util::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.shrink_to_fit();
        al.reset_alloc_detected();
        v.assign(tst.begin(), tst.end());
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(Ty::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 19 : 12));
        VERIFY(Ty::not_empty_count == (std::is_same<typename Src::value_type, Ty>::value ? 19 : 12));
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty, typename Src>
int test_range_less() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5, 6, 7};
        Src tst = {11, 12, 13, 14, 15};
        util::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.assign(tst.begin(), tst.end());
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(Ty::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 17 : 12));
        VERIFY(Ty::not_empty_count == (std::is_same<typename Src::value_type, Ty>::value ? 17 : 12));
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty, typename Src>
int test_range_same_amount() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        Src tst = {11, 12, 13, 14, 15};
        util::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.assign(tst.begin(), tst.end());
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(Ty::instance_count == (std::is_same<typename Src::value_type, Ty>::value ? 15 : 10));
        VERIFY(Ty::not_empty_count == (std::is_same<typename Src::value_type, Ty>::value ? 15 : 10));
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty, typename Src>
int test_range_empty_to_not_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        Src tst;
        util::vector<Ty, test_allocator<Ty>> v(tst_prev, al);
        v.assign(tst.begin(), tst.end());
        CHECK_EMPTY(v);
        VERIFY(Ty::instance_count == 5);
        VERIFY(Ty::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(Ty::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_range_empty_to_empty_random_access_range_assignable() {
    return test_range_empty_to_empty<T, std::vector<T>>();
}
int test_range_not_empty_to_empty_random_access_range_assignable() {
    return test_range_not_empty_to_empty<T, std::vector<T>>();
}
int test_range_more_no_realloc_random_access_range_assignable() {
    return test_range_more_no_realloc<T, std::vector<T>>();
}
int test_range_more_needs_realloc_random_access_range_assignable_nothrow_move() {
    return test_range_more_needs_realloc<T, std::vector<T>>();
}
int test_range_more_needs_realloc_random_access_range_assignable_throwing_move() {
    return test_range_more_needs_realloc<T_ThrowingMove, std::vector<T_ThrowingMove>>();
}
int test_range_less_random_access_range_assignable() { return test_range_less<T, std::vector<T>>(); }
int test_range_same_amount_random_access_range_assignable() { return test_range_same_amount<T, std::vector<T>>(); }
int test_range_empty_to_not_empty_random_access_range_assignable() {
    return test_range_empty_to_not_empty<T, std::vector<T>>();
}
int test_range_empty_to_empty_not_random_access_range_assignable() {
    return test_range_empty_to_empty<T_NotAssignable, std::vector<T_NotAssignable>>();
}
int test_range_not_empty_to_empty_random_access_range_not_assignable() {
    return test_range_not_empty_to_empty<T_NotAssignable, std::vector<T_NotAssignable>>();
}
int test_range_more_no_realloc_random_access_range_not_assignable() {
    return test_range_more_no_realloc<T_NotAssignable, std::vector<T_NotAssignable>>();
}
int test_range_more_needs_realloc_random_access_range_not_assignable_nothrow_move() {
    return test_range_more_needs_realloc<T_NotAssignable, std::vector<T_NotAssignable>>();
}
int test_range_more_needs_realloc_random_access_range_not_assignable_throwing_move() {
    return test_range_more_needs_realloc<T_ThrowingMove_NotAssignable, std::vector<T_ThrowingMove_NotAssignable>>();
}
int test_range_less_random_access_range_not_assignable() {
    return test_range_less<T_NotAssignable, std::vector<T_NotAssignable>>();
}
int test_range_same_amount_random_access_range_not_assignable() {
    return test_range_same_amount<T_NotAssignable, std::vector<T_NotAssignable>>();
}
int test_range_empty_to_not_empty_random_access_range_not_assignable() {
    return test_range_empty_to_not_empty<T_NotAssignable, std::vector<T_NotAssignable>>();
}

int test_range_empty_to_empty_generic_input_range_assignable() { return test_range_empty_to_empty<T, std::list<T>>(); }
int test_range_not_empty_to_empty_generic_input_range_assignable() {
    return test_range_not_empty_to_empty<T, std::list<T>>();
}
int test_range_more_no_realloc_generic_input_range_assignable() {
    return test_range_more_no_realloc<T, std::list<T>>();
}
int test_range_more_needs_realloc_generic_input_range_assignable_nothrow_move() {
    return test_range_more_needs_realloc<T, std::list<T>>();
}
int test_range_more_needs_realloc_generic_input_range_assignable_throwing_move() {
    return test_range_more_needs_realloc<T_ThrowingMove, std::list<T_ThrowingMove>>();
}
int test_range_less_generic_input_range_assignable() { return test_range_less<T, std::list<T>>(); }
int test_range_same_amount_generic_input_range_assignable() { return test_range_same_amount<T, std::list<T>>(); }
int test_range_empty_to_not_empty_generic_input_range_assignable() {
    return test_range_empty_to_not_empty<T, std::list<T>>();
}
int test_range_empty_to_empty_not_generic_input_range_assignable() {
    return test_range_empty_to_empty<T_NotAssignable, std::list<T_NotAssignable>>();
}
int test_range_not_empty_to_empty_generic_input_range_not_assignable() {
    return test_range_not_empty_to_empty<T_NotAssignable, std::list<T_NotAssignable>>();
}
int test_range_more_no_realloc_generic_input_range_not_assignable() {
    return test_range_more_no_realloc<T_NotAssignable, std::list<T_NotAssignable>>();
}
int test_range_more_needs_realloc_generic_input_range_not_assignable_nothrow_move() {
    return test_range_more_needs_realloc<T_NotAssignable, std::list<T_NotAssignable>>();
}
int test_range_more_needs_realloc_generic_input_range_not_assignable_throwing_move() {
    return test_range_more_needs_realloc<T_ThrowingMove_NotAssignable, std::list<T_ThrowingMove_NotAssignable>>();
}
int test_range_less_generic_input_range_not_assignable() {
    return test_range_less<T_NotAssignable, std::list<T_NotAssignable>>();
}
int test_range_same_amount_generic_input_range_not_assignable() {
    return test_range_same_amount<T_NotAssignable, std::list<T_NotAssignable>>();
}
int test_range_empty_to_not_empty_generic_input_range_not_assignable() {
    return test_range_empty_to_not_empty<T_NotAssignable, std::list<T_NotAssignable>>();
}

int test_range_empty_to_empty_random_access_range_assignable_int() {
    return test_range_empty_to_empty<T, std::vector<int>>();
}
int test_range_not_empty_to_empty_random_access_range_assignable_int() {
    return test_range_not_empty_to_empty<T, std::vector<int>>();
}
int test_range_more_no_realloc_random_access_range_assignable_int() {
    return test_range_more_no_realloc<T, std::vector<int>>();
}
int test_range_more_needs_realloc_random_access_range_assignable_nothrow_move_int() {
    return test_range_more_needs_realloc<T, std::vector<int>>();
}
int test_range_more_needs_realloc_random_access_range_assignable_throwing_move_int() {
    return test_range_more_needs_realloc<T_ThrowingMove, std::vector<int>>();
}
int test_range_less_random_access_range_assignable_int() { return test_range_less<T, std::vector<int>>(); }
int test_range_same_amount_random_access_range_assignable_int() {
    return test_range_same_amount<T, std::vector<int>>();
}
int test_range_empty_to_not_empty_random_access_range_assignable_int() {
    return test_range_empty_to_not_empty<T, std::vector<int>>();
}
int test_range_empty_to_empty_not_random_access_range_assignable_int() {
    return test_range_empty_to_empty<T_NotAssignable, std::vector<int>>();
}
int test_range_not_empty_to_empty_random_access_range_not_assignable_int() {
    return test_range_not_empty_to_empty<T_NotAssignable, std::vector<int>>();
}
int test_range_more_no_realloc_random_access_range_not_assignable_int() {
    return test_range_more_no_realloc<T_NotAssignable, std::vector<int>>();
}
int test_range_more_needs_realloc_random_access_range_not_assignable_nothrow_move_int() {
    return test_range_more_needs_realloc<T_NotAssignable, std::vector<int>>();
}
int test_range_more_needs_realloc_random_access_range_not_assignable_throwing_move_int() {
    return test_range_more_needs_realloc<T_ThrowingMove_NotAssignable, std::vector<int>>();
}
int test_range_less_random_access_range_not_assignable_int() {
    return test_range_less<T_NotAssignable, std::vector<int>>();
}
int test_range_same_amount_random_access_range_not_assignable_int() {
    return test_range_same_amount<T_NotAssignable, std::vector<int>>();
}
int test_range_empty_to_not_empty_random_access_range_not_assignable_int() {
    return test_range_empty_to_not_empty<T_NotAssignable, std::vector<int>>();
}

int test_range_empty_to_empty_generic_input_range_assignable_int() {
    return test_range_empty_to_empty<T, std::list<int>>();
}
int test_range_not_empty_to_empty_generic_input_range_assignable_int() {
    return test_range_not_empty_to_empty<T, std::list<int>>();
}
int test_range_more_no_realloc_generic_input_range_assignable_int() {
    return test_range_more_no_realloc<T, std::list<int>>();
}
int test_range_more_needs_realloc_generic_input_range_assignable_nothrow_move_int() {
    return test_range_more_needs_realloc<T, std::list<int>>();
}
int test_range_more_needs_realloc_generic_input_range_assignable_throwing_move_int() {
    return test_range_more_needs_realloc<T_ThrowingMove, std::list<int>>();
}
int test_range_less_generic_input_range_assignable_int() { return test_range_less<T, std::list<int>>(); }
int test_range_same_amount_generic_input_range_assignable_int() { return test_range_same_amount<T, std::list<int>>(); }
int test_range_empty_to_not_empty_generic_input_range_assignable_int() {
    return test_range_empty_to_not_empty<T, std::list<int>>();
}
int test_range_empty_to_empty_not_generic_input_range_assignable_int() {
    return test_range_empty_to_empty<T_NotAssignable, std::list<int>>();
}
int test_range_not_empty_to_empty_generic_input_range_not_assignable_int() {
    return test_range_not_empty_to_empty<T_NotAssignable, std::list<int>>();
}
int test_range_more_no_realloc_generic_input_range_not_assignable_int() {
    return test_range_more_no_realloc<T_NotAssignable, std::list<int>>();
}
int test_range_more_needs_realloc_generic_input_range_not_assignable_nothrow_move_int() {
    return test_range_more_needs_realloc<T_NotAssignable, std::list<int>>();
}
int test_range_more_needs_realloc_generic_input_range_not_assignable_throwing_move_int() {
    return test_range_more_needs_realloc<T_ThrowingMove_NotAssignable, std::list<int>>();
}
int test_range_less_generic_input_range_not_assignable_int() {
    return test_range_less<T_NotAssignable, std::list<int>>();
}
int test_range_same_amount_generic_input_range_not_assignable_int() {
    return test_range_same_amount<T_NotAssignable, std::list<int>>();
}
int test_range_empty_to_not_empty_generic_input_range_not_assignable_int() {
    return test_range_empty_to_not_empty<T_NotAssignable, std::list<int>>();
}

}  // namespace

ADD_TEST_CASE("", "vector", test_range_empty_to_empty_random_access_range_assignable);
ADD_TEST_CASE("", "vector", test_range_not_empty_to_empty_random_access_range_assignable);
ADD_TEST_CASE("", "vector", test_range_more_no_realloc_random_access_range_assignable);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_random_access_range_assignable_nothrow_move);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_random_access_range_assignable_throwing_move);
ADD_TEST_CASE("", "vector", test_range_less_random_access_range_assignable);
ADD_TEST_CASE("", "vector", test_range_same_amount_random_access_range_assignable);
ADD_TEST_CASE("", "vector", test_range_empty_to_not_empty_random_access_range_assignable);
ADD_TEST_CASE("", "vector", test_range_empty_to_empty_not_random_access_range_assignable);
ADD_TEST_CASE("", "vector", test_range_not_empty_to_empty_random_access_range_not_assignable);
ADD_TEST_CASE("", "vector", test_range_more_no_realloc_random_access_range_not_assignable);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_random_access_range_not_assignable_nothrow_move);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_random_access_range_not_assignable_throwing_move);
ADD_TEST_CASE("", "vector", test_range_less_random_access_range_not_assignable);
ADD_TEST_CASE("", "vector", test_range_same_amount_random_access_range_not_assignable);
ADD_TEST_CASE("", "vector", test_range_empty_to_not_empty_random_access_range_not_assignable);
ADD_TEST_CASE("", "vector", test_range_empty_to_empty_generic_input_range_assignable);
ADD_TEST_CASE("", "vector", test_range_not_empty_to_empty_generic_input_range_assignable);
ADD_TEST_CASE("", "vector", test_range_more_no_realloc_generic_input_range_assignable);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_generic_input_range_assignable_nothrow_move);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_generic_input_range_assignable_throwing_move);
ADD_TEST_CASE("", "vector", test_range_less_generic_input_range_assignable);
ADD_TEST_CASE("", "vector", test_range_same_amount_generic_input_range_assignable);
ADD_TEST_CASE("", "vector", test_range_empty_to_not_empty_generic_input_range_assignable);
ADD_TEST_CASE("", "vector", test_range_empty_to_empty_not_generic_input_range_assignable);
ADD_TEST_CASE("", "vector", test_range_not_empty_to_empty_generic_input_range_not_assignable);
ADD_TEST_CASE("", "vector", test_range_more_no_realloc_generic_input_range_not_assignable);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_generic_input_range_not_assignable_nothrow_move);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_generic_input_range_not_assignable_throwing_move);
ADD_TEST_CASE("", "vector", test_range_less_generic_input_range_not_assignable);
ADD_TEST_CASE("", "vector", test_range_same_amount_generic_input_range_not_assignable);
ADD_TEST_CASE("", "vector", test_range_empty_to_not_empty_generic_input_range_not_assignable);
ADD_TEST_CASE("", "vector", test_range_empty_to_empty_random_access_range_assignable_int);
ADD_TEST_CASE("", "vector", test_range_not_empty_to_empty_random_access_range_assignable_int);
ADD_TEST_CASE("", "vector", test_range_more_no_realloc_random_access_range_assignable_int);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_random_access_range_assignable_nothrow_move_int);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_random_access_range_assignable_throwing_move_int);
ADD_TEST_CASE("", "vector", test_range_less_random_access_range_assignable_int);
ADD_TEST_CASE("", "vector", test_range_same_amount_random_access_range_assignable_int);
ADD_TEST_CASE("", "vector", test_range_empty_to_not_empty_random_access_range_assignable_int);
ADD_TEST_CASE("", "vector", test_range_empty_to_empty_not_random_access_range_assignable_int);
ADD_TEST_CASE("", "vector", test_range_not_empty_to_empty_random_access_range_not_assignable_int);
ADD_TEST_CASE("", "vector", test_range_more_no_realloc_random_access_range_not_assignable_int);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_random_access_range_not_assignable_nothrow_move_int);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_random_access_range_not_assignable_throwing_move_int);
ADD_TEST_CASE("", "vector", test_range_less_random_access_range_not_assignable_int);
ADD_TEST_CASE("", "vector", test_range_same_amount_random_access_range_not_assignable_int);
ADD_TEST_CASE("", "vector", test_range_empty_to_not_empty_random_access_range_not_assignable_int);
ADD_TEST_CASE("", "vector", test_range_empty_to_empty_generic_input_range_assignable_int);
ADD_TEST_CASE("", "vector", test_range_not_empty_to_empty_generic_input_range_assignable_int);
ADD_TEST_CASE("", "vector", test_range_more_no_realloc_generic_input_range_assignable_int);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_generic_input_range_assignable_nothrow_move_int);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_generic_input_range_assignable_throwing_move_int);
ADD_TEST_CASE("", "vector", test_range_less_generic_input_range_assignable_int);
ADD_TEST_CASE("", "vector", test_range_same_amount_generic_input_range_assignable_int);
ADD_TEST_CASE("", "vector", test_range_empty_to_not_empty_generic_input_range_assignable_int);
ADD_TEST_CASE("", "vector", test_range_empty_to_empty_not_generic_input_range_assignable_int);
ADD_TEST_CASE("", "vector", test_range_not_empty_to_empty_generic_input_range_not_assignable_int);
ADD_TEST_CASE("", "vector", test_range_more_no_realloc_generic_input_range_not_assignable_int);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_generic_input_range_not_assignable_nothrow_move_int);
ADD_TEST_CASE("", "vector", test_range_more_needs_realloc_generic_input_range_not_assignable_throwing_move_int);
ADD_TEST_CASE("", "vector", test_range_less_generic_input_range_not_assignable_int);
ADD_TEST_CASE("", "vector", test_range_same_amount_generic_input_range_not_assignable_int);
ADD_TEST_CASE("", "vector", test_range_empty_to_not_empty_generic_input_range_not_assignable_int);
