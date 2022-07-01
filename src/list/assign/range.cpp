#include "list_tests.h"

#include "uxs/vector.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T, typename Src = uxs::vector<T>>
int test_range_empty_to_empty() {
    test_allocator<void> al;

    {
        Src src;
        uxs::list<Ty, test_allocator<Ty>> l(al);
        l.assign(src.begin(), src.end());
        CHECK_EMPTY(l);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T, typename Src = uxs::vector<T>>
int test_range_not_empty_to_empty() {
    test_allocator<void> al;

    {
        Src src = {1, 2, 3, 4, 5};
        uxs::list<Ty, test_allocator<Ty>> l(al);
        l.assign(src.begin(), src.end());
        CHECK(l, src.size(), src.begin());
        VERIFY(T::instance_count == 10);
        VERIFY(T::not_empty_count == 10);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T, typename Src = uxs::vector<T>>
int test_range_more() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        Src src = {11, 12, 13, 14, 15, 16, 17};
        uxs::list<Ty, test_allocator<Ty>> l(init, al);
        l.assign(src.begin(), src.end());
        CHECK(l, src.size(), src.begin());
        VERIFY(T::instance_count == 19);
        VERIFY(T::not_empty_count == 19);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 7);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T, typename Src = uxs::vector<T>>
int test_range_less() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5, 6, 7};
        Src src = {11, 12, 13, 14, 15};
        uxs::list<Ty, test_allocator<Ty>> l(init, al);
        l.assign(src.begin(), src.end());
        CHECK(l, src.size(), src.begin());
        VERIFY(T::instance_count == 17);
        VERIFY(T::not_empty_count == 17);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T, typename Src = uxs::vector<T>>
int test_range_same_amount() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        Src src = {11, 12, 13, 14, 15};
        uxs::list<Ty, test_allocator<Ty>> l(init, al);
        l.assign(src.begin(), src.end());
        CHECK(l, src.size(), src.begin());
        VERIFY(T::instance_count == 15);
        VERIFY(T::not_empty_count == 15);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T, typename Src = uxs::vector<T>>
int test_range_empty_to_not_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        Src src;
        uxs::list<Ty, test_allocator<Ty>> l(init, al);
        l.assign(src.begin(), src.end());
        CHECK_EMPTY(l);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_range_empty_to_empty);
ADD_TEST_CASE("", "list", test_range_not_empty_to_empty);
ADD_TEST_CASE("", "list", test_range_more);
ADD_TEST_CASE("", "list", test_range_less);
ADD_TEST_CASE("", "list", test_range_same_amount);
ADD_TEST_CASE("", "list", test_range_empty_to_not_empty);
