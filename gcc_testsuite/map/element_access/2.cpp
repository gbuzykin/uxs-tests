// { dg-do run { target c++11 } }

// 2010-11-10  Paolo Carlini  <paolo.carlini@oracle.com>
//
// Copyright (C) 2010-2022 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include "gcc_testsuite/util/testsuite_hooks.h"
#include "gcc_testsuite/util/testsuite_rvalref.h"

#include "uxs/map.h"

#include <iterator>

namespace {

int test01() {
    using __gnu_test::rvalstruct;

    typedef uxs::map<rvalstruct, rvalstruct> Map;

    Map m;
    VERIFY(m.empty());

    m[rvalstruct(1)] = rvalstruct(17);
    VERIFY(m.size() == 1);
    VERIFY((m.begin()->first).val == 1);
    VERIFY((m.begin()->second).val == 17);
    VERIFY(m[rvalstruct(1)].val == 17);

    m[rvalstruct(2)] = rvalstruct(9);
    VERIFY(m.size() == 2);
    VERIFY(m[rvalstruct(2)].val == 9);

    m[rvalstruct(1)] = rvalstruct(5);
    VERIFY(m.size() == 2);
    VERIFY(m[rvalstruct(1)].val == 5);
    VERIFY(m[rvalstruct(2)].val == 9);

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "map", test01);
