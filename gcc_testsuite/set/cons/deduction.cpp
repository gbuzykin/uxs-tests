// { dg-do compile { target c++17 } }

#if __cplusplus >= 201703L

#    include "gcc_testsuite/util/testsuite_allocator.h"
#    include "gcc_testsuite/util/testsuite_iterators.h"

#    include "uxs/set.h"

using __gnu_test::SimpleAllocator;
using value_type = uxs::set<int>::value_type;
using __gnu_test::input_iterator_wrapper;
using __gnu_test::test_container;

namespace {

static_assert(std::is_same_v<decltype(uxs::set{1, 2, 3}), uxs::set<int>>);

static_assert(std::is_same_v<decltype(uxs::set{1, 2, 3}), uxs::set<int>>);

static_assert(std::is_same_v<decltype(uxs::set{{1, 2, 3}, std::less<int>{}, {}}), uxs::set<int>>);

static_assert(std::is_same_v<decltype(uxs::set{{1, 2, 3}, std::less<int>{}}), uxs::set<int>>);

static_assert(std::is_same_v<decltype(uxs::set{{1, 2, 3}, SimpleAllocator<int>{}}),
                             uxs::set<int, std::less<int>, SimpleAllocator<int>>>);

static_assert(std::is_same_v<decltype(uxs::set{{1, 2, 3}, {}, SimpleAllocator<int>{}}),
                             uxs::set<int, std::less<int>, SimpleAllocator<int>>>);

#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wunused-function"
void test01() {
    uxs::set<int> x;

    static_assert(std::is_same_v<decltype(uxs::set(x.begin(), x.end())), uxs::set<int>>);

    static_assert(
        std::is_same_v<decltype(uxs::set{x.begin(), x.end(), std::less<int>{}, std::allocator<int>{}}), uxs::set<int>>);

    static_assert(std::is_same_v<decltype(uxs::set{x.begin(), x.end(), std::less<int>{}, {}}), uxs::set<int>>);

    static_assert(std::is_same_v<decltype(uxs::set(x.begin(), x.end(), std::less<int>{})), uxs::set<int>>);

    static_assert(std::is_same_v<decltype(uxs::set{x.begin(), x.end(), {}, std::allocator<int>{}}), uxs::set<int>>);

    static_assert(std::is_same_v<decltype(uxs::set{x.begin(), x.end(), SimpleAllocator<int>{}}),
                                 uxs::set<int, std::less<int>, SimpleAllocator<int>>>);

    static_assert(std::is_same_v<decltype(uxs::set{x.begin(), x.end(), {}, SimpleAllocator<int>{}}),
                                 uxs::set<int, std::less<int>, SimpleAllocator<int>>>);
}

void test02() {
    value_type array[1];
    test_container<value_type, input_iterator_wrapper> x(array);

    static_assert(std::is_same_v<decltype(uxs::set(x.begin(), x.end())), uxs::set<int>>);

    static_assert(std::is_same_v<decltype(uxs::set{x.begin(), x.end(), std::less<int>{}, std::allocator<value_type>{}}),
                                 uxs::set<int>>);

    static_assert(std::is_same_v<decltype(uxs::set{x.begin(), x.end(), std::less<int>{}, {}}), uxs::set<int>>);

    static_assert(std::is_same_v<decltype(uxs::set(x.begin(), x.end(), std::less<int>{})), uxs::set<int>>);

    static_assert(std::is_same_v<decltype(uxs::set{x.begin(), x.end(), std::allocator<value_type>{}}), uxs::set<int>>);

    static_assert(std::is_same_v<decltype(uxs::set{x.begin(), x.end(), SimpleAllocator<value_type>{}}),
                                 uxs::set<int, std::less<int>, SimpleAllocator<value_type>>>);

    static_assert(
        std::is_same_v<decltype(uxs::set{x.begin(), x.end(), {}, std::allocator<value_type>{}}), uxs::set<int>>);

    static_assert(std::is_same_v<decltype(uxs::set{x.begin(), x.end(), {}, SimpleAllocator<value_type>{}}),
                                 uxs::set<int, std::less<int>, SimpleAllocator<value_type>>>);
}
#    pragma GCC diagnostic pop

}  // namespace

#endif
