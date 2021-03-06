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

#include "gcc_testsuite/util/testsuite_allocator.h"
#include "gcc_testsuite/util/testsuite_hooks.h"

#include "uxs/multimap.h"

#include <string>

namespace {

using Cmp = std::less<int>;

using __gnu_test::uneq_allocator;

int test01() {
    typedef uneq_allocator<std::pair<const int, std::string>> alloc_type;
    typedef uxs::multimap<int, std::string, Cmp, alloc_type> test_type;
    test_type v1(alloc_type(1));
    const char* str = "A long enough string to require dynamic allocation";
    v1 = {{1, str}};

    alloc_type a2(2);
    test_type v2(std::move(v1), a2);

    VERIFY(1 == v1.get_allocator().get_personality());
    VERIFY(2 == v2.get_allocator().get_personality());

    VERIFY(v1.empty());
    VERIFY(v2.begin()->second == str);

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "multimap", test01);
