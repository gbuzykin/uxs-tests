// { dg-do compile { target c++17 } }

#if __cplusplus >= 201703L

#    include "gcc_testsuite/util/testsuite_allocator.h"
#    include "gcc_testsuite/util/testsuite_iterators.h"

#    include "uxs/multiset.h"

using __gnu_test::SimpleAllocator;
using value_type = uxs::multiset<int>::value_type;

namespace {

static_assert(std::is_same_v<decltype(uxs::multiset{1, 2, 3}), uxs::multiset<int>>);

static_assert(std::is_same_v<decltype(uxs::multiset{1, 2, 3}), uxs::multiset<int>>);

static_assert(std::is_same_v<decltype(uxs::multiset{{1, 2, 3}, std::less<int>{}, {}}), uxs::multiset<int>>);

static_assert(std::is_same_v<decltype(uxs::multiset{{1, 2, 3}, std::less<int>{}}), uxs::multiset<int>>);

static_assert(std::is_same_v<decltype(uxs::multiset{{1, 2, 3}, SimpleAllocator<int>{}}),
                             uxs::multiset<int, std::less<int>, SimpleAllocator<int>>>);

static_assert(std::is_same_v<decltype(uxs::multiset{{1, 2, 3}, {}, SimpleAllocator<int>{}}),
                             uxs::multiset<int, std::less<int>, SimpleAllocator<int>>>);

void f() {
    uxs::multiset<int> x;

    static_assert(std::is_same_v<decltype(uxs::multiset(x.begin(), x.end())), uxs::multiset<int>>);

    static_assert(std::is_same_v<decltype(uxs::multiset{x.begin(), x.end(), std::less<int>{}, std::allocator<int>{}}),
                                 uxs::multiset<int>>);

    static_assert(std::is_same_v<decltype(uxs::multiset{x.begin(), x.end(), std::less<int>{}, {}}), uxs::multiset<int>>);

    static_assert(std::is_same_v<decltype(uxs::multiset(x.begin(), x.end(), std::less<int>{})), uxs::multiset<int>>);

    static_assert(
        std::is_same_v<decltype(uxs::multiset{x.begin(), x.end(), std::allocator<int>{}}), uxs::multiset<int>>);

    static_assert(std::is_same_v<decltype(uxs::multiset{x.begin(), x.end(), SimpleAllocator<int>{}}),
                                 uxs::multiset<int, std::less<int>, SimpleAllocator<int>>>);

    static_assert(
        std::is_same_v<decltype(uxs::multiset{x.begin(), x.end(), {}, std::allocator<int>{}}), uxs::multiset<int>>);

    static_assert(std::is_same_v<decltype(uxs::multiset{x.begin(), x.end(), {}, SimpleAllocator<int>{}}),
                                 uxs::multiset<int, std::less<int>, SimpleAllocator<int>>>);
}

using __gnu_test::input_iterator_wrapper;
using __gnu_test::test_container;

void g() {
    value_type array[1];
    test_container<value_type, input_iterator_wrapper> x(array);

    static_assert(std::is_same_v<decltype(uxs::multiset(x.begin(), x.end())), uxs::multiset<int>>);

    static_assert(
        std::is_same_v<decltype(uxs::multiset{x.begin(), x.end(), std::less<int>{}, std::allocator<value_type>{}}),
                       uxs::multiset<int>>);

    static_assert(std::is_same_v<decltype(uxs::multiset{x.begin(), x.end(), std::less<int>{}, {}}), uxs::multiset<int>>);

    static_assert(std::is_same_v<decltype(uxs::multiset(x.begin(), x.end(), std::less<int>{})), uxs::multiset<int>>);

    static_assert(
        std::is_same_v<decltype(uxs::multiset{x.begin(), x.end(), std::allocator<value_type>{}}), uxs::multiset<int>>);

    static_assert(std::is_same_v<decltype(uxs::multiset{x.begin(), x.end(), SimpleAllocator<value_type>{}}),
                                 uxs::multiset<int, std::less<int>, SimpleAllocator<value_type>>>);

    static_assert(std::is_same_v<decltype(uxs::multiset{x.begin(), x.end(), {}, std::allocator<value_type>{}}),
                                 uxs::multiset<int>>);

    static_assert(std::is_same_v<decltype(uxs::multiset{x.begin(), x.end(), {}, SimpleAllocator<value_type>{}}),
                                 uxs::multiset<int, std::less<int>, SimpleAllocator<value_type>>>);
}

}  // namespace

#endif
