#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_copy_empty_to_empty() {
    unfriendly_test_allocator<void> al, al2;

    {
        uxs::list<Ty, unfriendly_test_allocator<Ty>> l(al), l_from(al2);
        l = l_from;
        CHECK_EMPTY(l);
        VERIFY(l.get_allocator() == al2);
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
int test_copy_not_empty_to_empty() {
    unfriendly_test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::list<Ty, unfriendly_test_allocator<Ty>> l(al), l_from(init, al2);
        l = l_from;
        CHECK(l, init.size(), init.begin());
        VERIFY(l.get_allocator() == al2);
        VERIFY(T::instance_count == 15);
        VERIFY(T::not_empty_count == 15);
        VERIFY(al.get_alloc_detected() == 0);
        VERIFY(al2.get_alloc_detected() == 10);
        VERIFY(al2.get_alloc_count() == 10);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_copy_not_empty_to_not_empty() {
    unfriendly_test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15, 16, 17};
        uxs::list<Ty, unfriendly_test_allocator<Ty>> l(init, al), l_from(init2, al2);
        l = l_from;
        CHECK(l, init2.size(), init2.begin());
        VERIFY(l.get_allocator() == al2);
        VERIFY(T::instance_count == 26);
        VERIFY(T::not_empty_count == 26);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 0);
        VERIFY(al2.get_alloc_detected() == 14);
        VERIFY(al2.get_alloc_count() == 14);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_copy_empty_to_not_empty() {
    unfriendly_test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::list<Ty, unfriendly_test_allocator<Ty>> l(init, al), l_from(al2);
        al.reset_alloc_detected();
        l = l_from;
        CHECK_EMPTY(l);
        VERIFY(l.get_allocator() == al2);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == 0);
        VERIFY(al2.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_copy_empty_to_empty);
ADD_TEST_CASE("", "list", test_copy_not_empty_to_empty);
ADD_TEST_CASE("", "list", test_copy_not_empty_to_not_empty);
ADD_TEST_CASE("", "list", test_copy_empty_to_not_empty);
