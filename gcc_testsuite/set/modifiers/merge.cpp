// Copyright (C) 2016-2022 Free Software Foundation, Inc.
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

// { dg-do run { target c++17 } }

#include "gcc_testsuite/util/testsuite_hooks.h"

#include "uxs/functional.h"
#include "uxs/multiset.h"
#include "uxs/set.h"

#include <algorithm>

namespace {

using test_type = uxs::set<int>;

int test01() {
    const test_type c0{1, 2, 3};
    test_type c1 = c0, c2 = c0;
    c1.merge(c2);
    VERIFY(c1 == c0);
    VERIFY(c2 == c0);

    c1.clear();
    c1.merge(c2);
    VERIFY(c1 == c0);
    VERIFY(c2.empty());

    c2.merge(std::move(c1));
    VERIFY(c1.empty());
    VERIFY(c2 == c0);

    return 0;
}

int test02() {
    const test_type c0{1, 2, 3};
    test_type c1 = c0;
    uxs::set<int, uxs::less<>> c2(c0.begin(), c0.end());
    c1.merge(c2);
    VERIFY(c1 == c0);
    VERIFY(c2.size() == c0.size() && std::equal(c2.begin(), c2.end(), c0.begin()));

    c1.clear();
    c1.merge(c2);
    VERIFY(c1 == c0);
    VERIFY(c2.empty());

    c2.merge(std::move(c1));
    VERIFY(c1.empty());
    VERIFY(c2.size() == c0.size() && std::equal(c2.begin(), c2.end(), c0.begin()));

    return 0;
}

int test03() {
    const test_type c0{1, 2, 3};
    test_type c1 = c0;
    uxs::set<int, uxs::greater<>> c2(c0.begin(), c0.end());
    c1.merge(c2);
    VERIFY(c1 == c0);
    VERIFY(c2.size() == c0.size() && std::equal(c2.rbegin(), c2.rend(), c0.begin()));

    c1.clear();
    c1.merge(c2);
    VERIFY(c1 == c0);
    VERIFY(c2.empty());

    c2.merge(c1);
    VERIFY(c1.empty());
    VERIFY(c2.size() == c0.size() && std::equal(c2.rbegin(), c2.rend(), c0.begin()));

    c1.merge(std::move(c2));
    VERIFY(c1 == c0);
    VERIFY(c2.empty());

    return 0;
}

int test04() {
    const test_type c0{1, 2, 3};
    test_type c1 = c0;
    uxs::multiset<int, uxs::greater<>> c2(c0.begin(), c0.end());
    c1.merge(c2);
    VERIFY(c1 == c0);
    VERIFY(c2.size() == c0.size() && std::equal(c2.rbegin(), c2.rend(), c0.begin()));

    c1.clear();
    c1.merge(c2);
    VERIFY(c1 == c0);
    VERIFY(c2.empty());

    c2.merge(c1);
    VERIFY(c1.empty());
    VERIFY(c2.size() == c0.size() && std::equal(c2.rbegin(), c2.rend(), c0.begin()));

    c1 = c0;
    c2.merge(c1);
    VERIFY(c1.empty());
    VERIFY(c2.size() == (2 * c0.size()));
    VERIFY(std::is_sorted(c2.begin(), c2.end(), c2.value_comp()));

    c1.merge(c2);
    VERIFY(c1 == c0);
    VERIFY(c2.size() == c0.size() && std::equal(c2.rbegin(), c2.rend(), c0.begin()));

    c1.merge(std::move(c2));
    VERIFY(c1 == c0);
    VERIFY(c2.size() == c0.size() && std::equal(c2.rbegin(), c2.rend(), c0.begin()));

    c1.clear();
    c1.merge(std::move(c2));
    VERIFY(c1 == c0);
    VERIFY(c2.empty());

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "set", test01);
ADD_TEST_CASE("", "set", test02);
ADD_TEST_CASE("", "set", test03);
ADD_TEST_CASE("", "set", test04);
