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

#include <uxs-legacy/multiset.h>
#include <uxs-legacy/set.h>

#include <algorithm>

namespace {

using test_type = uxs::multiset<int>;

int test01() {
    test_type c{1, 1, 2, 2, 3, 3};
    test_type::node_type node;
    test_type::iterator pos;

    node = c.extract(0);
    VERIFY(!node);
    VERIFY(node.empty());
    VERIFY(c.size() == 6);

    pos = c.insert(std::move(node));
    VERIFY(!node);
    VERIFY(node.empty());
    VERIFY(c.size() == 6);
    VERIFY(pos == c.end());

    node = c.extract(1);
    VERIFY((bool)node);
    VERIFY(!node.empty());
    VERIFY(c.size() == 5);
    VERIFY(node.get_allocator() == c.get_allocator());
    VERIFY(node.value() == 1);

    node.value() = 4;
    VERIFY(node.value() == 4);

    pos = c.insert(std::move(node));
    VERIFY(!node);
    VERIFY(node.empty());
    VERIFY(c.size() == 6);
    VERIFY(pos != c.end());
    VERIFY(*pos == 4);
    VERIFY(c.count(1) == 1);
    VERIFY(c.count(4) == 1);
    VERIFY(std::is_sorted(c.begin(), c.end()));

    pos = c.insert(c.begin(), std::move(node));
    VERIFY(!node);
    VERIFY(node.empty());
    VERIFY(c.size() == 6);
    VERIFY(pos == c.end());

    node = c.extract(1);
    pos = c.insert(c.begin(), std::move(node));
    VERIFY(!node);
    VERIFY(c.size() == 6);
    VERIFY(pos != c.end());
    VERIFY(*pos == 1);

    test_type c2 = c;
    node = c2.extract(1);
    pos = c.insert(std::move(node));
    VERIFY(pos != c.end());
    VERIFY(node.empty());
    VERIFY(*pos == 1);

    return 0;
}

int test02() {
    test_type c{1, 1, 2, 2, 3, 3};
    test_type::node_type node;
    test_type::iterator pos;

    node = c.extract(c.begin());
    VERIFY(!node.empty());
    VERIFY(c.size() == 5);
    VERIFY(node.get_allocator() == c.get_allocator());
    VERIFY(node.value() == 1);

    pos = c.insert(std::next(c.begin()), std::move(node));
    VERIFY(node.empty());
    VERIFY(c.size() == 6);
    VERIFY(pos != c.end());
    VERIFY(*pos == 1);
    VERIFY(pos == std::next(c.begin()));

    return 0;
}

int test03() {
    struct less : std::less<int> {};
    using compat_type1 = uxs::multiset<int, less>;
    static_assert(std::is_same<test_type::node_type, compat_type1::node_type>::value, "");
    using compat_type2 = uxs::set<int>;
    static_assert(std::is_same<test_type::node_type, compat_type2::node_type>::value, "");
    using compat_type3 = uxs::set<int, less>;
    static_assert(std::is_same<test_type::node_type, compat_type3::node_type>::value, "");
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "multiset", test01);
ADD_TEST_CASE("", "multiset", test02);
ADD_TEST_CASE("", "multiset", test03);
