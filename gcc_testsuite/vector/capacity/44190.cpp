// Copyright (C) 2010-2022 Free Software Foundation, Inc.
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

// { dg-options "-D_GLIBCXX_DEBUG_PEDANTIC" }

#include "uxs/vector.h"

#include "gcc_testsuite/util/testsuite_hooks.h"

namespace {

// libstdc++/44190
int test01() {
    uxs::vector<int> v;
    v.resize(10);
    VERIFY(v.size() <= v.capacity());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
