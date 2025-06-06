// Copyright (C) 2008-2022 Free Software Foundation, Inc.
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

// NB: This issue affected only debug-mode, in pedantic mode.

// { dg-options "-D_GLIBCXX_DEBUG_PEDANTIC" }

#include "test_suite.h"

#include "uxs/vector.h"

#include <algorithm>
#include <iterator>

namespace {

// http://gcc.gnu.org/ml/libstdc++/2008-05/msg00046.html
int test01() {
    typedef uxs::vector<unsigned> array_t;
    typedef std::back_insert_iterator<array_t> bii_t;

    array_t a;

    // Push 5 elements.
    a.push_back(0);
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    // Ensure that there is enough space for other two elements.
    // (2 + 5 = 7)
    if (a.capacity() < 7) { a.reserve(7); }
    // Add two new elements.
    std::copy(a.begin(), a.begin() + 2, bii_t(a));

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
