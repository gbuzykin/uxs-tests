// Copyright (C) 2013-2022 Free Software Foundation, Inc.
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

#include "gcc_testsuite/util/testsuite_allocators.h"
#include "gcc_testsuite/util/testsuite_hooks.h"

#include <memory>

using __gnu_test::CustomPointerAlloc;

namespace {

struct T {
    int i;
};

}  // namespace

template class uxs::vector<T, CustomPointerAlloc<T>>;

namespace {

int test01() {
    typedef CustomPointerAlloc<T> alloc_type;
    typedef uxs::vector<T, alloc_type> test_type;
    test_type v;
    v.push_back(T());
    VERIFY(++v.begin() == v.end());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
