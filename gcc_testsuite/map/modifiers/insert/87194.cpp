// Copyright (C) 2018-2022 Free Software Foundation, Inc.
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

// { dg-do run { target c++11 } }

#include "gcc_testsuite/util/testsuite_hooks.h"

#include <uxs-legacy/map.h>

#include <iterator>

namespace {

struct S {
    operator std::pair<const int, int>() && {
        int i = val;
        val = 0;
        return {i, 0};
    }

    int val;
};

int test01() {
    S a[3] = {{1}, {2}, {3}};
    uxs::map<int, int> s;
    s.insert(std::make_move_iterator(a), std::make_move_iterator(a + 3));
    VERIFY(s.size() == 3);
    VERIFY(s.find(0) == s.end());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "map", test01);
