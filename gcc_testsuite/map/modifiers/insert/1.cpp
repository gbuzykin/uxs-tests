// 2001-08-23 pme & Sylvain.Pion@sophia.inria.fr

// Copyright (C) 2001-2022 Free Software Foundation, Inc.
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

// 23.3.1.2, table 69 -- map::insert(p,t)

#include "gcc_testsuite/util/testsuite_hooks.h"

#include <uxs-legacy/map.h>

// { dg-do run }

namespace {

// libstdc++/3349 and
// http://gcc.gnu.org/ml/gcc-patches/2001-08/msg01375.html
int test01() {
    typedef uxs::map<int, int> Map;
    Map M;
    Map::iterator hint;

    hint = M.insert(Map::value_type(7, 0)).first;

    M.insert(hint, Map::value_type(8, 1));
    M.insert(M.begin(), Map::value_type(9, 2));

    VERIFY(M.find(7) != M.end());
    VERIFY(M.find(8) != M.end());
    VERIFY(M.find(9) != M.end());

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "map", test01);
