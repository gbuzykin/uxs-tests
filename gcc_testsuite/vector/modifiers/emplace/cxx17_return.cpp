// Copyright (C) 2016-2022 Free Software Foundation, Inc.
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

// { dg-do run { target c++17 } }

#include "gcc_testsuite/util/testsuite_hooks.h"
#include "util/vector.h"

using test_type = util::vector<int>;

namespace {

int test01() {
    test_type x{1, 2, 3, 4};
    test_type::reference r = x.emplace_back(5);
    VERIFY(r == 5);
    VERIFY(&r == &x.back());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
