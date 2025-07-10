// { dg-do run { target c++11 } }

// Copyright (C) 2009-2022 Free Software Foundation, Inc.
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

#include <uxs-legacy/list.h>

namespace {

// PR libstdc++/42352
int test01() {
    uxs::list<int> l{3, 2, 4, 1, 5, 9, 0, 8, 6, 7};

    l.sort();

    for (auto it = l.begin(); it != l.end(); ++it) {
        static int nn = 0;
        VERIFY(*it == nn++);
    }

    return 0;
}

int test02() {
    uxs::list<int> l{3, 2, 4, 1, 5, 9, 0, 8, 6, 7};

    struct compare {
        bool operator()(const int& one, const int& two) const { return one > two; }
    };

    l.sort(compare());

    for (auto it = l.begin(); it != l.end(); ++it) {
        static int nn = 9;
        VERIFY(*it == nn--);
    }

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test01);
ADD_TEST_CASE("", "list", test02);
