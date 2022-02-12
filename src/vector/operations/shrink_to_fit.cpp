#include "vector_tests.h"

using namespace util_test_suite;

namespace {

template<typename Ty>
int test_shrink_to_fit_empty() {
    test_allocator<void> al;

    {
        util::vector<Ty, test_allocator<Ty>> v(al);
        v.shrink_to_fit();
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    return 0;
}

template<typename Ty>
int test_shrink_to_fit_not_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst = {1, 2, 3, 4, 5};
        util::vector<Ty, test_allocator<Ty>> v(tst, al);
        v.reserve(v.capacity() + 10);
        al.reset_alloc_detected();

        v.shrink_to_fit();
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() == v.size());
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == v.size());
        v.shrink_to_fit();
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() == v.size());
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == v.size());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_shrink_to_fit_not_empty_but_cleared() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst = {1, 2, 3, 4, 5};
        util::vector<Ty, test_allocator<Ty>> v(tst, al);
        al.reset_alloc_detected();

        v.clear();
        v.shrink_to_fit();
        VERIFY(v.capacity() == 0);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_shrink_to_fit_empty_nothrow_move() { return test_shrink_to_fit_empty<T>(); }
int test_shrink_to_fit_not_empty_nothrow_move() { return test_shrink_to_fit_not_empty<T>(); }
int test_shrink_to_fit_not_empty_but_cleared_nothrow_move() { return test_shrink_to_fit_not_empty_but_cleared<T>(); }
int test_shrink_to_fit_empty_throwing_move() { return test_shrink_to_fit_empty<T_ThrowingMove>(); }
int test_shrink_to_fit_not_empty_throwing_move() { return test_shrink_to_fit_not_empty<T_ThrowingMove>(); }
int test_shrink_to_fit_not_empty_but_cleared_throwing_move() {
    return test_shrink_to_fit_not_empty_but_cleared<T_ThrowingMove>();
}

}  // namespace

ADD_TEST_CASE("", "vector", test_shrink_to_fit_empty_nothrow_move);
ADD_TEST_CASE("", "vector", test_shrink_to_fit_not_empty_nothrow_move);
ADD_TEST_CASE("", "vector", test_shrink_to_fit_not_empty_but_cleared_nothrow_move);
ADD_TEST_CASE("", "vector", test_shrink_to_fit_empty_throwing_move);
ADD_TEST_CASE("", "vector", test_shrink_to_fit_not_empty_throwing_move);
ADD_TEST_CASE("", "vector", test_shrink_to_fit_not_empty_but_cleared_throwing_move);
