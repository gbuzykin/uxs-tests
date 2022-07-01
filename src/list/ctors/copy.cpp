#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_copy_from_empty_propagate_alloc() {
    test_allocator<void> al;

    {
        uxs::list<Ty, test_allocator<Ty>> l_from(al);
        uxs::list<Ty, test_allocator<Ty>> l(l_from);
        CHECK_EMPTY(l);
        VERIFY(l.get_allocator() == al);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    return 0;
}

template<typename Ty = T>
int test_copy_from_empty_new_alloc() {
    test_allocator<void> al, al2;

    {
        uxs::list<Ty, test_allocator<Ty>> l_from(al2);
        uxs::list<Ty, test_allocator<Ty>> l(l_from, al);
        CHECK_EMPTY(l);
        VERIFY(l.get_allocator() == al);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
        VERIFY(al2.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    VERIFY(al2.get_alloc_detected() == 0);
    return 0;
}

template<typename Ty = T>
int test_copy_from_not_empty_propagate_alloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::list<Ty, test_allocator<Ty>> l_from(init, al);
        uxs::list<Ty, test_allocator<Ty>> l(l_from);
        CHECK(l, init.size(), init.begin());
        VERIFY(l.get_allocator() == al);
        VERIFY(T::instance_count == 15);
        VERIFY(T::not_empty_count == 15);
        VERIFY(al.get_alloc_detected() == 10);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_copy_from_not_empty_new_alloc() {
    test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::list<Ty, test_allocator<Ty>> l_from(init, al2);
        uxs::list<Ty, test_allocator<Ty>> l(l_from, al);
        CHECK(l, init.size(), init.begin());
        VERIFY(l.get_allocator() == al);
        VERIFY(T::instance_count == 15);
        VERIFY(T::not_empty_count == 15);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al2.get_alloc_detected() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_copy_from_empty_propagate_alloc);
ADD_TEST_CASE("", "list", test_copy_from_empty_new_alloc);
ADD_TEST_CASE("", "list", test_copy_from_not_empty_propagate_alloc);
ADD_TEST_CASE("", "list", test_copy_from_not_empty_new_alloc);
