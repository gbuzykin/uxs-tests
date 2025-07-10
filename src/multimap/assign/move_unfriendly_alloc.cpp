#include "rbtree_tests.h"

#include <uxs-legacy/multimap.h>

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_move_empty_to_empty() {
    unfriendly_test_allocator<void> al, al2;

    {
        uxs::multimap<Ty, Ty, std::less<Ty>, unfriendly_test_allocator<Ty>> ms(al), ms_from(al2);
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
        std::initializer_list<std::pair<const Ty, Ty>> init = {{1, 10}, {2, 10}, {3, 10}, {4, 10}, {5, 10}};
        uxs::multimap<Ty, Ty, std::less<Ty>, unfriendly_test_allocator<Ty>> ms(al), ms_from(init, al2);
        ms = std::move(ms_from);
        CHECK(ms, init.size(), init.begin());
        VERIFY(ms.get_allocator() == al);
        VERIFY(ms_from.get_allocator() == al2);
        VERIFY(T::instance_count == 20);
        VERIFY(T::not_empty_count == 20);
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
        std::initializer_list<std::pair<const Ty, Ty>> init = {{1, 10}, {2, 10}, {3, 10}, {4, 10}, {5, 10}};
        std::initializer_list<std::pair<const Ty, Ty>> init2 = {{11, 10}, {12, 10}, {13, 10}, {14, 10},
                                                                {15, 10}, {16, 10}, {17, 10}};
        uxs::multimap<Ty, Ty, std::less<Ty>, unfriendly_test_allocator<Ty>> ms(init, al), ms_from(init2, al2);
        ms = std::move(ms_from);
        CHECK(ms, init2.size(), init2.begin());
        VERIFY(ms.get_allocator() == al);
        VERIFY(ms_from.get_allocator() == al2);
        VERIFY(T::instance_count == 38);
        VERIFY(T::not_empty_count == 38);
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
        std::initializer_list<std::pair<const Ty, Ty>> init = {{1, 10}, {2, 10}, {3, 10}, {4, 10},
                                                               {5, 10}, {6, 10}, {7, 10}};
        std::initializer_list<std::pair<const Ty, Ty>> init2 = {{11, 10}, {12, 10}, {13, 10}, {14, 10}, {15, 10}};
        uxs::multimap<Ty, Ty, std::less<Ty>, unfriendly_test_allocator<Ty>> ms(init, al), ms_from(init2, al2);
        ms = std::move(ms_from);
        CHECK(ms, init2.size(), init2.begin());
        VERIFY(ms.get_allocator() == al);
        VERIFY(ms_from.get_allocator() == al2);
        VERIFY(T::instance_count == 34);
        VERIFY(T::not_empty_count == 34);
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
        std::initializer_list<std::pair<const Ty, Ty>> init = {{1, 10}, {2, 10}, {3, 10}, {4, 10}, {5, 10}};
        std::initializer_list<std::pair<const Ty, Ty>> init2 = {{11, 10}, {12, 10}, {13, 10}, {14, 10}, {15, 10}};
        uxs::multimap<Ty, Ty, std::less<Ty>, unfriendly_test_allocator<Ty>> ms(init, al), ms_from(init2, al2);
        ms = std::move(ms_from);
        CHECK(ms, init2.size(), init2.begin());
        VERIFY(ms.get_allocator() == al);
        VERIFY(ms_from.get_allocator() == al2);
        VERIFY(T::instance_count == 30);
        VERIFY(T::not_empty_count == 30);
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
        std::initializer_list<std::pair<const Ty, Ty>> init = {{1, 10}, {2, 10}, {3, 10}, {4, 10}, {5, 10}};
        uxs::multimap<Ty, Ty, std::less<Ty>, unfriendly_test_allocator<Ty>> ms(init, al), ms_from(al2);
        ms = std::move(ms_from);
        CHECK_EMPTY(ms);
        VERIFY(ms.get_allocator() == al);
        VERIFY(ms_from.get_allocator() == al2);
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
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

ADD_TEST_CASE("", "multimap", test_move_empty_to_empty);
ADD_TEST_CASE("", "multimap", test_move_not_empty_to_empty);
ADD_TEST_CASE("", "multimap", test_move_more);
ADD_TEST_CASE("", "multimap", test_move_less);
ADD_TEST_CASE("", "multimap", test_move_same_amount);
ADD_TEST_CASE("", "multimap", test_move_empty_to_not_empty);
