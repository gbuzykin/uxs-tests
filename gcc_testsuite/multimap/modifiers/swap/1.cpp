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

#include "uxs/multimap.h"

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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
bool operator<(T l, T r) { return l.i < r.i; }
#pragma GCC diagnostic pop

int swap_calls;

}  // namespace

namespace uxs {
template<>
void multimap<T, int>::swap(multimap<T, int>&) NOTHROW {
    ++swap_calls;
}
}  // namespace uxs

namespace {

// Should use multimap specialization for swap.
int test01() {
    uxs::multimap<T, int> A;
    uxs::multimap<T, int> B;
    swap_calls = 0;
    std::swap(A, B);
    VERIFY(1 == swap_calls);
    return 0;
}

// Should use multimap specialization for swap.
int test02() {
    using namespace std;
    uxs::multimap<T, int> A;
    uxs::multimap<T, int> B;
    swap_calls = 0;
    swap(A, B);
    VERIFY(1 == swap_calls);
    return 0;
}

}  // namespace

// See c++/13658 for background info.
ADD_TEST_CASE("", "multimap", test01);
ADD_TEST_CASE("", "multimap", test02);
