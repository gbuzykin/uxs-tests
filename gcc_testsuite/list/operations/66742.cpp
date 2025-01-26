// { dg-do run { target c++11 } }

#include "gcc_testsuite/util/testsuite_allocator.h"
#include "gcc_testsuite/util/testsuite_hooks.h"

#include "uxs/list.h"

namespace {

// PR libstdc++/66742
// abort on sorting list with custom allocator that is not stateless

template<typename List, typename Cmp = std::less<typename List::value_type>>
bool is_sorted(const List& l, Cmp cmp = {}) {
    auto it = l.begin();
    auto next = it;
    const auto end = l.end();
    if (it == end) { return true; }
    while (++next != end) {
        if (cmp(*next, *it)) {
            return false;
        } else {
            it = next;
        }
    }
    return true;
}

int test01() {
    using Alloc = __gnu_test::uneq_allocator<int>;
    Alloc a1(1);
    uxs::list<int, Alloc> l(a1);
    for (int i = 0; i < 1000; ++i) {
        l.push_front(i);
        l.push_back(i + (i % 3));
    }
    const auto orig = l;

    l.sort();
    VERIFY(is_sorted(l));
    l.sort();
    VERIFY(is_sorted(l));

    l = orig;
    l.sort(std::less<int>());
    VERIFY(is_sorted(l));
    l.sort(std::greater<int>());
    VERIFY(is_sorted(l, std::greater<int>()));

    return 0;
}

int test02() {
    // The standard doesn't require comparisons to be const-correct.
    // The initial fix for PR 66742 caused a regression here.

    struct X {
        bool operator<(X&) /* non-const */ { return false; }
    };

    struct Cmp {
        bool operator()(X&, X&) /* non-const */ { return false; }
    };

    uxs::list<X> l;
    l.sort();
    Cmp c;
    l.sort(c);

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test01);
ADD_TEST_CASE("", "list", test02);
