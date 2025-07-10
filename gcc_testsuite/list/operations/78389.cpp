// { dg-do run { target c++11 } }

// Copyright (C) 2017-2022 Free Software Foundation, Inc.
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

// 23.2.2.4 list operations [lib.list.ops]

#include "gcc_testsuite/util/testsuite_hooks.h"

#include <uxs-legacy/list.h>

namespace {

struct ThrowingComparator {
    ThrowingComparator(unsigned n) : throw_after(n), count(0) {}
    unsigned int throw_after;
    unsigned int count;
    bool operator()(int, int) {
        if (++count >= throw_after) { throw 666; }
        return false;
    }
};

struct X {
    X() = default;
    X(int) {}
};

unsigned int throw_after_X = 0;
unsigned int count_X = 0;

bool operator<(const X&, const X&) {
    if (++count_X >= throw_after_X) { throw 666; }
    return false;
}

int test01() {
    uxs::list<int> a{1, 2, 3, 4};
    uxs::list<int> b{5, 6, 7, 8, 9, 10, 11, 12};
    try {
        a.merge(b, ThrowingComparator{4});
    } catch (...) {}
    VERIFY(a.size() == static_cast<size_t>(std::distance(a.begin(), a.end())) &&
           b.size() == static_cast<size_t>(std::distance(b.begin(), b.end())));
    uxs::list<X> ax{1, 2, 3, 4};
    uxs::list<X> bx{5, 6, 7, 8, 9, 10, 11, 12};
    throw_after_X = 4;
    try {
        ax.merge(bx);
    } catch (...) {}
    VERIFY(ax.size() == static_cast<size_t>(std::distance(ax.begin(), ax.end())) &&
           bx.size() == static_cast<size_t>(std::distance(bx.begin(), bx.end())));
    uxs::list<int> ay{5, 6, 7, 8, 9, 10, 11, 12};
    try {
        ay.sort(ThrowingComparator{5});
    } catch (...) {}
    VERIFY(ay.size() == 8);
    uxs::list<X> az{5, 6, 7, 8, 9, 10, 11, 12};
    throw_after_X = 5;
    try {
        az.sort();
    } catch (...) {}
    VERIFY(az.size() == 8);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test01);
