// { dg-do run { target c++11 } }

// Copyright (C) 2011-2022 Free Software Foundation, Inc.
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

#include "uxs/vector.h"

#include "gcc_testsuite/util/testsuite_allocators.h"
#include "gcc_testsuite/util/testsuite_hooks.h"

using __gnu_test::propagating_allocator;

namespace {

int test01() {
    typedef propagating_allocator<int, true> alloc_type;
    alloc_type alloc(5);

    uxs::vector<int, alloc_type> v(10u, 1, alloc);
    v.reserve(100);
    VERIFY(v.size() < v.capacity());
    v.shrink_to_fit();
    VERIFY(v.size() == v.capacity());
    VERIFY(v.get_allocator().get_personality() == alloc.get_personality());

    return 0;
}

int test02() {
    typedef propagating_allocator<int, false> alloc_type;
    alloc_type alloc(5);

    uxs::vector<int, alloc_type> v(10u, 1, alloc);
    v.reserve(100);
    VERIFY(v.size() < v.capacity());
    v.shrink_to_fit();
    VERIFY(v.size() == v.capacity());
    VERIFY(v.get_allocator().get_personality() == alloc.get_personality());

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
ADD_TEST_CASE("", "vector", test02);
