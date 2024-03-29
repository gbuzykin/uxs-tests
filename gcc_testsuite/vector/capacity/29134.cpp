// Copyright (C) 2006-2022 Free Software Foundation, Inc.
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

// 23.2.4.2 vector capacity [lib.vector.capacity]

#include "gcc_testsuite/util/testsuite_allocator.h"
#include "gcc_testsuite/util/testsuite_hooks.h"

#include "uxs/vector.h"

namespace {

// libstdc++/29134
int test01() {
    uxs::vector<int> v;

    std::allocator<int> a = v.get_allocator();
#if __cplusplus >= 202002L
    // std::allocator_traits::max_size() is unrealistically large,
    // uxs::vector::max_size() returns a smaller value.
    VERIFY(v.max_size() <= __gnu_test::max_size(a));
#else
    VERIFY(v.max_size() == __gnu_test::max_size(a));
#endif

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
