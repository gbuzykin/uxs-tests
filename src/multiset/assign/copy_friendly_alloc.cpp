#include "rbtree_tests.h"

#include <uxs-legacy/multiset.h>

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_copy_empty_to_empty() {
    test_allocator<void> al, al2;

    {
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(al), ms_from(al2);
        ms = ms_from;
        CHECK_EMPTY(ms);
        VERIFY(ms.get_allocator() == al);
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
    test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(al), ms_from(init, al2);
        ms = ms_from;
        CHECK(ms, init.size(), init.begin());
        VERIFY(ms.get_allocator() == al);
        VERIFY(T::instance_count == 15);
        VERIFY(T::not_empty_count == 15);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 5);
        VERIFY(al2.get_alloc_detected() == 5);
        VERIFY(al2.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_copy_more() {
    test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15, 16, 17};
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(init, al), ms_from(init2, al2);
        ms = ms_from;
        CHECK(ms, init2.size(), init2.begin());
        VERIFY(ms.get_allocator() == al);
        VERIFY(T::instance_count == 26);
        VERIFY(T::not_empty_count == 26);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 7);
        VERIFY(al2.get_alloc_detected() == 7);
        VERIFY(al2.get_alloc_count() == 7);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_copy_less() {
    test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5, 6, 7};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15};
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(init, al), ms_from(init2, al2);
        ms = ms_from;
        CHECK(ms, init2.size(), init2.begin());
        VERIFY(ms.get_allocator() == al);
        VERIFY(T::instance_count == 22);
        VERIFY(T::not_empty_count == 22);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 5);
        VERIFY(al2.get_alloc_detected() == 5);
        VERIFY(al2.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_copy_same_amount() {
    test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15};
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(init, al), ms_from(init2, al2);
        ms = ms_from;
        CHECK(ms, init2.size(), init2.begin());
        VERIFY(ms.get_allocator() == al);
        VERIFY(T::instance_count == 20);
        VERIFY(T::not_empty_count == 20);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 5);
        VERIFY(al2.get_alloc_detected() == 5);
        VERIFY(al2.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_copy_empty_to_not_empty() {
    test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(init, al), ms_from(al2);
        ms = ms_from;
        CHECK_EMPTY(ms);
        VERIFY(ms.get_allocator() == al);
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

ADD_TEST_CASE("", "multiset", test_copy_empty_to_empty);
ADD_TEST_CASE("", "multiset", test_copy_not_empty_to_empty);
ADD_TEST_CASE("", "multiset", test_copy_more);
ADD_TEST_CASE("", "multiset", test_copy_less);
ADD_TEST_CASE("", "multiset", test_copy_same_amount);
ADD_TEST_CASE("", "multiset", test_copy_empty_to_not_empty);
