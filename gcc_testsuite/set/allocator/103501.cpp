// { dg-do run { target c++11 } }

// PR libstdc++/103501

#include "gcc_testsuite/util/testsuite_allocator.h"
#include "gcc_testsuite/util/testsuite_hooks.h"

#include "uxs/set.h"

namespace {

struct X {
    int i;

    X(int i) : i(i) {}
    X(const X& x) noexcept : i(x.i) {}
    X(X&& x) noexcept : i(x.i) { x.i = -1; }

    bool operator<(const X& rhs) const { return i < rhs.i; }
};

int test01() {
    using Alloc = __gnu_test::uneq_allocator<X>;
    uxs::set<X, std::less<X>, Alloc> s1{{1, 2, 3}, Alloc(1)};
    uxs::set<X, std::less<X>, Alloc> s2{std::move(s1), Alloc(2)};
    const X* prev = nullptr;
    for (const X& x : s1) {
        if (prev) VERIFY(*prev < x);
        prev = &x;
    }
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "set", test01);
