// Copyright (C) 2004-2022 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include "gcc_testsuite/util/testsuite_hooks.h"

#include "uxs/multiset.h"

namespace {

#if __cplusplus >= 201103L
#    define NOTHROW noexcept
#else
#    define NOTHROW
#endif

struct T {
    int i;
};

// T must be LessThanComparable to pass concept-checks
bool operator<(T l, T r) { return l.i < r.i; }

int swap_calls;

}  // namespace

namespace uxs {
template<>
void multiset<T>::swap(multiset<T>&) NOTHROW {
    ++swap_calls;
}
}  // namespace uxs

namespace {

// Should use multiset specialization for swap.
int test01() {
    uxs::multiset<T> A;
    uxs::multiset<T> B;
    swap_calls = 0;
    std::swap(A, B);
    VERIFY(1 == swap_calls);
    return 0;
}

// Should use multiset specialization for swap.
int test02() {
    using namespace std;
    uxs::multiset<T> A;
    uxs::multiset<T> B;
    swap_calls = 0;
    swap(A, B);
    VERIFY(1 == swap_calls);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "multiset", test01);
ADD_TEST_CASE("", "multiset", test02);
