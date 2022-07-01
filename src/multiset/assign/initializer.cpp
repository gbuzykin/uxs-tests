#include "rbtree_tests.h"

#include "uxs/multiset.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_initializer_empty_to_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init;
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(al);
        ms = init;
        CHECK_EMPTY(ms);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_initializer_not_empty_to_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(al);
        ms = init;
        CHECK(ms, init.size(), init.begin());
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_initializer_more() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15, 16, 17};
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(init, al);
        ms = init2;
        CHECK(ms, init2.size(), init2.begin());
        VERIFY(T::instance_count == 19);
        VERIFY(T::not_empty_count == 19);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 7);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_initializer_less() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5, 6, 7};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15};
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(init, al);
        ms = init2;
        CHECK(ms, init2.size(), init2.begin());
        VERIFY(T::instance_count == 17);
        VERIFY(T::not_empty_count == 17);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_initializer_same_amount() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15};
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(init, al);
        ms = init2;
        CHECK(ms, init2.size(), init2.begin());
        VERIFY(T::instance_count == 15);
        VERIFY(T::not_empty_count == 15);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_initializer_empty_to_not_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> init2;
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(init, al);
        ms = init2;
        CHECK_EMPTY(ms);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_initializer_assign_func() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5, 6, 7};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15};
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(init, al);
        ms.assign(init2);
        CHECK(ms, init2.size(), init2.begin());
        VERIFY(T::instance_count == 17);
        VERIFY(T::not_empty_count == 17);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "multiset", test_initializer_empty_to_empty);
ADD_TEST_CASE("", "multiset", test_initializer_not_empty_to_empty);
ADD_TEST_CASE("", "multiset", test_initializer_more);
ADD_TEST_CASE("", "multiset", test_initializer_less);
ADD_TEST_CASE("", "multiset", test_initializer_same_amount);
ADD_TEST_CASE("", "multiset", test_initializer_empty_to_not_empty);
ADD_TEST_CASE("", "multiset", test_initializer_assign_func);
