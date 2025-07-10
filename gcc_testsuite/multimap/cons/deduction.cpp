// { dg-do compile { target c++17 } }

#if __cplusplus >= 201703L

#    include "gcc_testsuite/util/testsuite_allocator.h"
#    include "gcc_testsuite/util/testsuite_iterators.h"

#    include <uxs-legacy/multimap.h>

using __gnu_test::SimpleAllocator;
using value_type = uxs::multimap<int, double>::value_type;

static_assert(
    std::is_same_v<decltype(uxs::multimap{value_type{1, 2.0}, {2, 3.0}, {3, 4.0}}), uxs::multimap<int, double>>);

static_assert(std::is_same_v<decltype(uxs::multimap{std::pair{1, 2.0}, {2, 3.0}, {3, 4.0}}), uxs::multimap<int, double>>);

static_assert(
    std::is_same_v<decltype(uxs::multimap{{value_type{1, 2.0}, {2, 3.0}, {3, 4.0}}}), uxs::multimap<int, double>>);

static_assert(
    std::is_same_v<decltype(uxs::multimap{{std::pair{1, 2.0}, {2, 3.0}, {3, 4.0}}}), uxs::multimap<int, double>>);

static_assert(std::is_same_v<decltype(uxs::multimap{{value_type{1, 2.0}, {2, 3.0}, {3, 4.0}}, std::less<int>{}, {}}),
                             uxs::multimap<int, double>>);

static_assert(std::is_same_v<decltype(uxs::multimap{{std::pair{1, 2.0}, {2, 3.0}, {3, 4.0}}, std::less<int>{}, {}}),
                             uxs::multimap<int, double>>);

/* This is not deducible, {} could be deduced as _Compare or _Allocator.
static_assert(std::is_same_v<
          decltype(uxs::multimap{{value_type{1, 2.0}, {2, 3.0}, {3, 4.0}},
                     {}}),
          uxs::multimap<int, double>>);
*/

static_assert(std::is_same_v<decltype(uxs::multimap{{std::pair{1, 2.0}, {2, 3.0}, {3, 4.0}}, std::less<int>{}}),
                             uxs::multimap<int, double>>);

static_assert(
    std::is_same_v<decltype(uxs::multimap{{value_type{1, 2.0}, {2, 3.0}, {3, 4.0}}, {}, SimpleAllocator<value_type>{}}),
                   uxs::multimap<int, double, std::less<int>, SimpleAllocator<value_type>>>);

static_assert(
    std::is_same_v<decltype(uxs::multimap{{std::pair{1, 2.0}, {2, 3.0}, {3, 4.0}}, {}, SimpleAllocator<value_type>{}}),
                   uxs::multimap<int, double, std::less<int>, SimpleAllocator<value_type>>>);

void f() {
    uxs::multimap<int, double> x;
    static_assert(std::is_same_v<decltype(uxs::multimap(x.begin(), x.end())), uxs::multimap<int, double>>);

    static_assert(
        std::is_same_v<decltype(uxs::multimap{x.begin(), x.end(), std::less<int>{}, std::allocator<value_type>{}}),
                       uxs::multimap<int, double>>);

    static_assert(
        std::is_same_v<decltype(uxs::multimap{x.begin(), x.end(), std::less<int>{}, {}}), uxs::multimap<int, double>>);

    static_assert(
        std::is_same_v<decltype(uxs::multimap(x.begin(), x.end(), std::less<int>{})), uxs::multimap<int, double>>);

    static_assert(std::is_same_v<decltype(uxs::multimap{x.begin(), x.end(), {}, std::allocator<value_type>{}}),
                                 uxs::multimap<int, double>>);

    static_assert(std::is_same_v<decltype(uxs::multimap{x.begin(), x.end(), {}, SimpleAllocator<value_type>{}}),
                                 uxs::multimap<int, double, std::less<int>, SimpleAllocator<value_type>>>);
}

using __gnu_test::input_iterator_wrapper;
using __gnu_test::test_container;

void g() {
    value_type array[1];
    test_container<value_type, input_iterator_wrapper> x(array);

    static_assert(std::is_same_v<decltype(uxs::multimap(x.begin(), x.end())), uxs::multimap<int, double>>);

    static_assert(
        std::is_same_v<decltype(uxs::multimap{x.begin(), x.end(), std::less<int>{}, std::allocator<value_type>{}}),
                       uxs::multimap<int, double>>);

    static_assert(
        std::is_same_v<decltype(uxs::multimap{x.begin(), x.end(), std::less<int>{}, {}}), uxs::multimap<int, double>>);

    static_assert(
        std::is_same_v<decltype(uxs::multimap(x.begin(), x.end(), std::less<int>{})), uxs::multimap<int, double>>);

    static_assert(std::is_same_v<decltype(uxs::multimap{x.begin(), x.end(), {}, std::allocator<value_type>{}}),
                                 uxs::multimap<int, double>>);

    static_assert(std::is_same_v<decltype(uxs::multimap{x.begin(), x.end(), {}, SimpleAllocator<value_type>{}}),
                                 uxs::multimap<int, double, std::less<int>, SimpleAllocator<value_type>>>);
}

void h() {
    std::pair<int, double> array[1];
    test_container<std::pair<int, double>, input_iterator_wrapper> x(array);

    static_assert(std::is_same_v<decltype(uxs::multimap(x.begin(), x.end())), uxs::multimap<int, double>>);

    static_assert(
        std::is_same_v<decltype(uxs::multimap{x.begin(), x.end(), std::less<int>{}, std::allocator<value_type>{}}),
                       uxs::multimap<int, double>>);

    static_assert(
        std::is_same_v<decltype(uxs::multimap{x.begin(), x.end(), std::less<int>{}, {}}), uxs::multimap<int, double>>);

    static_assert(
        std::is_same_v<decltype(uxs::multimap(x.begin(), x.end(), std::less<int>{})), uxs::multimap<int, double>>);

    static_assert(std::is_same_v<decltype(uxs::multimap{x.begin(), x.end(), {}, std::allocator<value_type>{}}),
                                 uxs::multimap<int, double>>);

    static_assert(std::is_same_v<decltype(uxs::multimap{x.begin(), x.end(), {}, SimpleAllocator<value_type>{}}),
                                 uxs::multimap<int, double, std::less<int>, SimpleAllocator<value_type>>>);
}

#endif
