// 1999-06-29 bkoz

// Copyright (C) 1999-2022 Free Software Foundation, Inc.
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

// 23.2.4.1 vector constructors, copy, and assignment

#include "gcc_testsuite/util/testsuite_allocators.h"
#include "gcc_testsuite/util/testsuite_hooks.h"

#include "uxs/vector.h"

namespace {

template<typename T>
struct A {};

struct B {};

int test01() {
    // 1
    uxs::vector<A<B>> vec01;
    uxs::vector<A<B>> vec02(5);
    typedef uxs::vector<A<B>>::size_type size_type;

    vec01 = vec02;
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
