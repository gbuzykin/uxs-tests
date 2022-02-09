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

// { dg-do compile { target c++11 } }

#include "gcc_testsuite/util/testsuite_allocators.h"
#include "util/vector.h"

using __gnu_test::propagating_allocator;

namespace {

struct T {
    int i;
};

int test01() {
    typedef std::allocator<T> alloc_type;
    typedef util::vector<T, alloc_type> test_type;
    test_type v1;
    test_type v2;
    // this is a GNU extension for std::allocator
    static_assert(noexcept(v1 = std::move(v2)), "Move assign cannot throw");
    static_assert(noexcept(v1.swap(v2)), "Swap cannot throw");
    return 0;
}

int test02() {
    typedef propagating_allocator<T, false> alloc_type;
    typedef util::vector<T, alloc_type> test_type;
    test_type v1(alloc_type(1));
    test_type v2(alloc_type(2));
    static_assert(!noexcept(v1 = std::move(v2)), "Move assign can throw");
    static_assert(noexcept(v1.swap(v2)), "Swap cannot throw");
    return 0;
}

int test03() {
    typedef propagating_allocator<T, true> alloc_type;
    typedef util::vector<T, alloc_type> test_type;
    test_type v1(alloc_type(1));
    test_type v2(alloc_type(2));
    static_assert(noexcept(v1 = std::move(v2)), "Move assign cannot throw");
    static_assert(noexcept(v1.swap(v2)), "Swap cannot throw");
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
ADD_TEST_CASE("", "vector", test02);
ADD_TEST_CASE("", "vector", test03);
