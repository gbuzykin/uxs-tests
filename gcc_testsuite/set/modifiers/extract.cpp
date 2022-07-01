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

#include "uxs/multiset.h"
#include "uxs/set.h"

#include <algorithm>

namespace {

using test_type = uxs::set<int>;

int test01() {
    test_type c{1, 2, 3};
    test_type::node_type node;
    test_type::insert_return_type ins;
    test_type::iterator pos;

    node = c.extract(0);
    VERIFY(!node);
    VERIFY(node.empty());
    VERIFY(c.size() == 3);

    ins = c.insert(std::move(node));
    VERIFY(!node);
    VERIFY(node.empty());
    VERIFY(c.size() == 3);
    VERIFY(!ins.inserted);
    VERIFY(!ins.node);
    VERIFY(ins.position == c.end());

    node = c.extract(1);
    VERIFY((bool)node);
    VERIFY(!node.empty());
    VERIFY(c.size() == 2);
    VERIFY(node.get_allocator() == c.get_allocator());
    VERIFY(node.value() == 1);

    node.value() = 4;
    VERIFY(node.value() == 4);

    ins = c.insert(std::move(node));
    VERIFY(!node);
    VERIFY(node.empty());
    VERIFY(c.size() == 3);
    VERIFY(ins.inserted);
    VERIFY(!ins.node);
    VERIFY(ins.position != c.end());
    VERIFY(*ins.position == 4);
    VERIFY(c.count(1) == 0);
    VERIFY(c.count(4) == 1);
    VERIFY(std::is_sorted(c.begin(), c.end()));

    pos = c.insert(c.begin(), std::move(node));
    VERIFY(!node);
    VERIFY(node.empty());
    VERIFY(c.size() == 3);
    VERIFY(pos == c.end());

    node = c.extract(2);
    pos = c.insert(c.begin(), std::move(node));
    VERIFY(c.size() == 3);
    VERIFY(pos != c.end());
    VERIFY(*pos == 2);

    test_type c2 = c;
    node = c2.extract(3);
    ins = c.insert(std::move(node));
    VERIFY(node.empty());
    VERIFY(ins.position != c.end());
    VERIFY(!ins.inserted);
    VERIFY(!ins.node.empty());
    VERIFY(ins.node.value() == 3);
    VERIFY(*ins.position == ins.node.value());

    return 0;
}

int test02() {
    test_type c{1, 2, 3};
    test_type::node_type node;
    test_type::insert_return_type ins;

    node = c.extract(c.begin());
    VERIFY((bool)node);
    VERIFY(!node.empty());
    VERIFY(c.size() == 2);
    VERIFY(node.get_allocator() == c.get_allocator());
    VERIFY(node.value() == 1);

    ins = c.insert(std::move(node));
    VERIFY(node.empty());
    VERIFY(c.size() == 3);
    VERIFY(ins.inserted);
    VERIFY(!ins.node);
    VERIFY(ins.position != c.end());
    VERIFY(*ins.position == 1);

    return 0;
}

int test03() {
    struct less : std::less<int> {};
    using compat_type1 = uxs::set<int, less>;
    static_assert(std::is_same<test_type::node_type, compat_type1::node_type>::value, "");
    using compat_type2 = uxs::multiset<int>;
    static_assert(std::is_same<test_type::node_type, compat_type2::node_type>::value, "");
    using compat_type3 = uxs::multiset<int, less>;
    static_assert(std::is_same<test_type::node_type, compat_type3::node_type>::value, "");

    return 0;
}

#if __cplusplus >= 201703L
void test04() {
    // Check order of members in insert_return_type
    auto [pos, ins, node] = test_type::insert_return_type{};
    static_assert(std::is_same<test_type::iterator, decltype(pos)>::value);
    static_assert(std::is_same<bool, decltype(ins)>::value);
    static_assert(std::is_same<test_type::node_type, decltype(node)>::value);
}
#endif

}  // namespace

ADD_TEST_CASE("", "set", test01);
ADD_TEST_CASE("", "set", test02);
ADD_TEST_CASE("", "set", test03);
