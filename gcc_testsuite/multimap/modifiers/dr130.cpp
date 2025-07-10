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

// NOTE: This makes use of the fact that we know how moveable
// is implemented on multiset (via swap). If the implementation changed
// this test may begin to fail.

#include "gcc_testsuite/util/testsuite_hooks.h"

#include <uxs-legacy/multimap.h>
#include <uxs-legacy/vector.h>

namespace {

int test01() {
    uxs::multimap<int, int> mm0;
    typedef uxs::multimap<int, int>::iterator iterator;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
    typedef uxs::multimap<int, int>::const_iterator const_iterator;
#pragma GCC diagnostic pop
    typedef uxs::multimap<int, int>::value_type value_type;
    typedef iterator insert_return_type;

    uxs::vector<insert_return_type> irt;
    for (int i = 1; i <= 4; ++i) {
        for (int j = 1; j <= i; ++j) { irt.push_back(mm0.insert(value_type(i, i))); }
    }

    iterator pos1 = mm0.erase(irt[1]);
    VERIFY(pos1 == irt[2]);

    iterator pos2 = mm0.erase(irt[2]);
    VERIFY(pos2 == irt[3]);

    iterator pos3 = mm0.erase(irt[9]);
    VERIFY(pos3 == mm0.end());

    return 0;
}

int test02() {
    uxs::multimap<int, int> mm0;
    typedef uxs::multimap<int, int>::iterator iterator;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
    typedef uxs::multimap<int, int>::const_iterator const_iterator;
#pragma GCC diagnostic pop
    typedef uxs::multimap<int, int>::value_type value_type;
    typedef iterator insert_return_type;

    uxs::vector<insert_return_type> irt;
    for (int i = 1; i <= 4; ++i) {
        for (int j = 1; j <= i; ++j) { irt.push_back(mm0.insert(value_type(i, i))); }
    }

    iterator pos1 = mm0.erase(irt[3], irt[6]);
    VERIFY(pos1 == irt[6]);

    iterator pos2 = mm0.erase(irt[6], ++irt[9]);
    VERIFY(pos2 == mm0.end());

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "multimap", test01);
ADD_TEST_CASE("", "multimap", test02);
