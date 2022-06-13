#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

int test_splice() {
    test_allocator<void> al;

    {
        uxs::list<T, test_allocator<T>> list1{{1, 2, 3, 4, 5}, al};
        uxs::list<T, test_allocator<T>> list2{{10, 20, 30, 40, 50}, al};

        auto it = list1.begin();
        std::advance(it, 2);

        std::initializer_list<T> tst1 = {1, 2, 10, 20, 30, 40, 50, 3, 4, 5};
        list1.splice(it, list2);
        CHECK(list1, tst1.size(), tst1.begin());
        VERIFY(list2.size() == 0);
        VERIFY(list2.begin() == list2.end());

        std::initializer_list<T> tst2 = {1, 2, 10, 20, 30, 40, 50};
        std::initializer_list<T> tst3 = {3, 4, 5};
        list2.splice(list2.begin(), list1, it, list1.end());
        CHECK(list1, tst2.size(), tst2.begin());
        CHECK(list2, tst3.size(), tst3.begin());

        std::initializer_list<T> tst4 = {1, 2, 5, 10, 20, 30, 40, 50};
        std::initializer_list<T> tst5 = {3, 4};
        list1.splice(std::next(list1.begin(), 2), list2, std::next(list2.begin(), 2));
        CHECK(list1, tst4.size(), tst4.begin());
        CHECK(list2, tst5.size(), tst5.begin());

        std::initializer_list<T> tst6 = {1, 2, 20, 5, 10, 30, 40, 50};
        list1.splice(std::next(list1.begin(), 2), list1, std::next(list1.begin(), 4));
        CHECK(list1, tst6.size(), tst6.begin());

        std::initializer_list<T> tst7 = {1, 2, 5, 10, 30, 40, 50, 20};
        list1.splice(std::next(list1.begin(), 2), list1, std::next(list1.begin(), 3), list1.end());
        CHECK(list1, tst7.size(), tst7.begin());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_sort() {
    test_allocator<void> al;

    {
        uxs::list<T, test_allocator<T>> list1{{5, 9, 0, 1, 3}, al};
        std::initializer_list<T> tst1 = {0, 1, 3, 5, 9};
        list1.sort();
        CHECK(list1, tst1.size(), tst1.begin());

        uxs::list<T, test_allocator<T>> list2{{8, 7, 2, 6, 4}, al};
        std::initializer_list<T> tst2 = {2, 4, 6, 7, 8};
        list2.sort();
        CHECK(list2, tst2.size(), tst2.begin());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_merge() {
    test_allocator<void> al;

    {
        uxs::list<T, test_allocator<T>> list1{{0, 1, 3, 5, 9}, al};
        uxs::list<T, test_allocator<T>> list2{{2, 4, 6, 7, 8}, al};
        std::initializer_list<T> tst3 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        list1.merge(list2);
        CHECK(list1, tst3.size(), tst3.begin());
        VERIFY(list2.size() == 0);
        VERIFY(list2.begin() == list2.end());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_remove() {
    test_allocator<void> al;

    {
        uxs::list<T, test_allocator<T>> list3{{0, 1, 2, 3, 4, 5, 6, 7, 3, 2}, al};
        list3.remove(2);
        std::initializer_list<T> tst4 = {0, 1, 3, 4, 5, 6, 7, 3};
        CHECK(list3, tst4.size(), tst4.begin());
        list3.remove_if([](decltype(*list3.begin()) el) { return el == 3; });
        std::initializer_list<T> tst5 = {0, 1, 4, 5, 6, 7};
        CHECK(list3, tst5.size(), tst5.begin());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_unique() {
    test_allocator<void> al;

    {
        uxs::list<T, test_allocator<T>> list4{{0, 1, 2, 2, 2, 5, 6, 7, 3, 3}, al};
        list4.unique();
        std::initializer_list<T> tst6 = {0, 1, 2, 5, 6, 7, 3};
        CHECK(list4, tst6.size(), tst6.begin());
        return 0;
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_splice);
ADD_TEST_CASE("", "list", test_sort);
ADD_TEST_CASE("", "list", test_merge);
ADD_TEST_CASE("", "list", test_remove);
ADD_TEST_CASE("", "list", test_unique);
