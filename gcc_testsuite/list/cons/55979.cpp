// { dg-do compile { target c++11 } }

// Copyright (C) 2013-2022 Free Software Foundation, Inc.
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

#include "uxs/list.h"

namespace {

struct A {
    A(int) {}
    A(const A&) = delete;
    A& operator=(const A&) = delete;
};

int test01() {
    int i[] = {1, 2};
    uxs::list<A> li(i, i + 2);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test01);