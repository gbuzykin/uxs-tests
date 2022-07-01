#include "rbtree_tests.h"

#include "uxs/multiset.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_move_from_empty_move_alloc() {
    test_allocator<void> al;

    {
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms_from(al);
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(std::move(ms_from));
        CHECK_EMPTY(ms);
        VERIFY(ms.get_allocator() == al);
        CHECK_EMPTY(ms_from);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    return 0;
}

template<typename Ty = T>
int test_move_from_empty_new_alloc() {
    test_allocator<void> al, al2;

    {
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms_from(al2);
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(std::move(ms_from), al);
        CHECK_EMPTY(ms);
        VERIFY(ms.get_allocator() == al);
        CHECK_EMPTY(ms_from);
        VERIFY(ms_from.get_allocator() == al2);
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
int test_move_from_empty_same_alloc() {
    test_allocator<void> al;

    {
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms_from(al);
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(std::move(ms_from), al);
        CHECK_EMPTY(ms);
        VERIFY(ms.get_allocator() == al);
        CHECK_EMPTY(ms_from);
        VERIFY(ms_from.get_allocator() == al);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    return 0;
}

template<typename Ty = T>
int test_move_from_not_empty_move_alloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms_from(init, al);
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(std::move(ms_from));
        CHECK(ms, init.size(), init.begin());
        VERIFY(ms.get_allocator() == al);
        CHECK_EMPTY(ms_from);
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_move_from_not_empty_new_alloc() {
    test_allocator<void> al, al2;

    {  // different allocators -> per-element movement
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms_from(init, al2);
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(std::move(ms_from), al);
        CHECK(ms, init.size(), init.begin());
        VERIFY(ms.get_allocator() == al);
        VERIFY(ms_from.size() == 0);
        VERIFY(ms_from.get_allocator() == al2);
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al2.get_alloc_detected() == 5);
        VERIFY(al2.get_alloc_count() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_move_from_not_empty_same_alloc() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms_from(init, al);
        uxs::multiset<Ty, std::less<Ty>, test_allocator<Ty>> ms(std::move(ms_from), al);
        CHECK(ms, init.size(), init.begin());
        VERIFY(ms.get_allocator() == al);
        CHECK_EMPTY(ms_from);
        VERIFY(ms_from.get_allocator() == al);
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "multiset", test_move_from_empty_move_alloc);
ADD_TEST_CASE("", "multiset", test_move_from_empty_new_alloc);
ADD_TEST_CASE("", "multiset", test_move_from_empty_same_alloc);
ADD_TEST_CASE("", "multiset", test_move_from_not_empty_move_alloc);
ADD_TEST_CASE("", "multiset", test_move_from_not_empty_new_alloc);
ADD_TEST_CASE("", "multiset", test_move_from_not_empty_same_alloc);
