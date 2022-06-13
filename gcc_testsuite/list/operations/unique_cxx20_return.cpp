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

// { dg-options "-std=gnu++2a" }
// { dg-do run { target c++2a } }

#include "gcc_testsuite/util/testsuite_hooks.h"

#include "uxs/list.h"

namespace {

using test_type = uxs::list<int>;

int test01() {
    test_type x{1, 2, 2, 4, 4, 2, 1};
    static_assert(std::is_same<decltype(x.unique()), test_type::size_type>::value, "");
    test_type::size_type r = x.unique();
    VERIFY(r == 2);
    r = x.unique();
    VERIFY(r == 0);
    return 0;
}

int test02() {
    auto pred = [](int val, int prev) { return val == prev; };
    test_type x{1, 2, 2, 4, 4, 2, 1};
    static_assert(std::is_same<decltype(x.unique(pred)), test_type::size_type>::value, "");
    test_type::size_type r = x.unique(pred);
    VERIFY(r == 2);
    r = x.unique(pred);
    VERIFY(r == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test01);
ADD_TEST_CASE("", "list", test02);
