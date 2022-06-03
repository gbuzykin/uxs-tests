// Copyright (C) 2020-2022 Free Software Foundation, Inc.
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

#include "uxs/vector.h"

#include "gcc_testsuite/util/testsuite_hooks.h"

namespace {

template<typename Container>
void test(std::initializer_list<typename Container::value_type> vals) {
    Container c{vals};
    c = std::move(c);
    VERIFY(c == c);

    auto it = c.begin();
    it = std::move(it);
    VERIFY(it == c.begin());
}

int test01() {
    test<uxs::vector<int>>({1, 2, 3});
    test<uxs::vector<uxs::vector<int>>>({{1, 2}, {3, 4}, {5, 6}, {7, 8}});
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
