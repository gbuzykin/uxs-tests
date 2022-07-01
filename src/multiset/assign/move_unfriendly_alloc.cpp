#include "rbtree_tests.h"

#include "uxs/multiset.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_move_empty_to_empty() {
    unfriendly_test_allocator<void> al, al2;

    {
        uxs::multiset<Ty, std::less<Ty>, unfriendly_test_allocator<Ty>> ms(al), ms_from(al2);
        ms = std::move(ms_from);
        CHECK_EMPTY(ms);
        VERIFY(ms.get_allocator() == al);
        VERIFY(ms_from.get_allocator() == al2);
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
    unfriendly_test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::multiset<Ty, std::less<Ty>, unfriendly_test_allocator<Ty>> ms(al), ms_from(init, al2);
        ms = std::move(ms_from);
        CHECK(ms, init.size(), init.begin());
        VERIFY(ms.get_allocator() == al);
        VERIFY(ms_from.get_allocator() == al2);
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 5);
        VERIFY(al2.get_alloc_detected() == 5);
        VERIFY(al2.get_alloc_count() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_move_more() {
    unfriendly_test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15, 16, 17};
        uxs::multiset<Ty, std::less<Ty>, unfriendly_test_allocator<Ty>> ms(init, al), ms_from(init2, al2);
        ms = std::move(ms_from);
        CHECK(ms, init2.size(), init2.begin());
        VERIFY(ms.get_allocator() == al);
        VERIFY(ms_from.get_allocator() == al2);
        VERIFY(T::instance_count == 19);
        VERIFY(T::not_empty_count == 19);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 7);
        VERIFY(al2.get_alloc_detected() == 7);
        VERIFY(al2.get_alloc_count() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_move_less() {
    unfriendly_test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5, 6, 7};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15};
        uxs::multiset<Ty, std::less<Ty>, unfriendly_test_allocator<Ty>> ms(init, al), ms_from(init2, al2);
        ms = std::move(ms_from);
        CHECK(ms, init2.size(), init2.begin());
        VERIFY(ms.get_allocator() == al);
        VERIFY(ms_from.get_allocator() == al2);
        VERIFY(T::instance_count == 17);
        VERIFY(T::not_empty_count == 17);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 5);
        VERIFY(al2.get_alloc_detected() == 5);
        VERIFY(al2.get_alloc_count() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_move_same_amount() {
    unfriendly_test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> init2 = {11, 12, 13, 14, 15};
        uxs::multiset<Ty, std::less<Ty>, unfriendly_test_allocator<Ty>> ms(init, al), ms_from(init2, al2);
        ms = std::move(ms_from);
        CHECK(ms, init2.size(), init2.begin());
        VERIFY(ms.get_allocator() == al);
        VERIFY(ms_from.get_allocator() == al2);
        VERIFY(T::instance_count == 15);
        VERIFY(T::not_empty_count == 15);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 5);
        VERIFY(al2.get_alloc_detected() == 5);
        VERIFY(al2.get_alloc_count() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_move_empty_to_not_empty() {
    unfriendly_test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::multiset<Ty, std::less<Ty>, unfriendly_test_allocator<Ty>> ms(init, al), ms_from(al2);
        ms = std::move(ms_from);
        CHECK_EMPTY(ms);
        VERIFY(ms.get_allocator() == al);
        VERIFY(ms_from.get_allocator() == al2);
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

ADD_TEST_CASE("", "multiset", test_move_empty_to_empty);
ADD_TEST_CASE("", "multiset", test_move_not_empty_to_empty);
ADD_TEST_CASE("", "multiset", test_move_more);
ADD_TEST_CASE("", "multiset", test_move_less);
ADD_TEST_CASE("", "multiset", test_move_same_amount);
ADD_TEST_CASE("", "multiset", test_move_empty_to_not_empty);
