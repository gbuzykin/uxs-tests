#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_move_empty_to_empty() {
    test_allocator<void> al, al2;

    {
        uxs::list<Ty, test_allocator<Ty>> l(al), l_from(al2);
        l = std::move(l_from);
        CHECK_EMPTY(l);
        VERIFY(l.get_allocator() == al2);
        CHECK_EMPTY(l_from);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
        VERIFY(al2.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_move_not_empty_to_empty() {
    test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> tst = {1, 2, 3, 4, 5};
        uxs::list<Ty, test_allocator<Ty>> l(al), l_from(tst, al2);
        l = std::move(l_from);
        CHECK(l, tst.size(), tst.begin());
        VERIFY(l.get_allocator() == al2);
        CHECK_EMPTY(l_from);
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == 0);
        VERIFY(al2.get_alloc_detected() == 5);
        VERIFY(al2.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_move_not_empty_to_not_empty() {
    test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> tst = {11, 12, 13, 14, 15, 16, 17};
        uxs::list<Ty, test_allocator<Ty>> l(tst_prev, al), l_from(tst, al2);
        l = std::move(l_from);
        CHECK(l, tst.size(), tst.begin());
        VERIFY(l.get_allocator() == al2);
        CHECK_EMPTY(l_from);
        VERIFY(T::instance_count == 19);
        VERIFY(T::not_empty_count == 19);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 0);
        VERIFY(al2.get_alloc_detected() == 7);
        VERIFY(al2.get_alloc_count() == 7);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_move_empty_to_not_empty() {
    test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        uxs::list<Ty, test_allocator<Ty>> l(tst_prev, al), l_from(al2);
        l = std::move(l_from);
        CHECK_EMPTY(l);
        VERIFY(l.get_allocator() == al2);
        CHECK_EMPTY(l_from);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 0);
        VERIFY(al2.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_move_empty_to_empty);
ADD_TEST_CASE("", "list", test_move_not_empty_to_empty);
ADD_TEST_CASE("", "list", test_move_not_empty_to_not_empty);
ADD_TEST_CASE("", "list", test_move_empty_to_not_empty);
