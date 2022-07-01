// { dg-do run { target c++11 } }

// PR libstdc++/103501

#include "gcc_testsuite/util/testsuite_allocator.h"
#include "gcc_testsuite/util/testsuite_hooks.h"

#include "uxs/multiset.h"

namespace {

struct Y {
    int i;

    Y(int i) : i(i) {}
    Y(const Y& y) noexcept : i(y.i) {}
    Y(Y&& y) noexcept : i(y.i) { y.i = -y.i; }

    bool operator<(const Y& rhs) const { return i < rhs.i; }
};

int test01() {
    using Alloc = __gnu_test::uneq_allocator<Y>;
    uxs::multiset<Y, std::less<Y>, Alloc> s1{{1, 2, 3}, Alloc(1)};
    uxs::multiset<Y, std::less<Y>, Alloc> s2{std::move(s1), Alloc(2)};
    const Y* prev = nullptr;
    for (const Y& y : s1) {
        if (prev) VERIFY(!(y < *prev));
        prev = &y;
    }
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "multiset", test01);
