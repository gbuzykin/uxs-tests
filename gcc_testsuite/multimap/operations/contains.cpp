// Copyright (C) 2018-2022 Free Software Foundation, Inc.
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

// { dg-options "-std=gnu++2a" }
// { dg-do run { target c++2a } }

#include "gcc_testsuite/util/testsuite_hooks.h"

#include "uxs/functional.h"
#include "uxs/multimap.h"

namespace {

int test01() {
    uxs::multimap<int, void*> m;
    VERIFY(!m.contains(0));
    VERIFY(!m.contains(1));
    m.emplace(0, nullptr);
    VERIFY(m.contains(0));
    VERIFY(!m.contains(1));
    m.emplace(0, nullptr);
    VERIFY(m.contains(0));
    VERIFY(!m.contains(1));
    m.emplace(1, nullptr);
    VERIFY(m.contains(0));
    VERIFY(m.contains(1));
    return 0;
}

struct Zero {};
bool operator<(Zero, int i) { return 0 < i; }
bool operator<(int i, Zero) { return i < 0; }

struct One {};
bool operator<(One, int i) { return 1 < i; }
bool operator<(int i, One) { return i < 1; }

int test02() {
    uxs::multimap<int, void*, uxs::less<>> m;
    VERIFY(!m.contains(Zero{}));
    VERIFY(!m.contains(One{}));
    m.emplace(0, nullptr);
    VERIFY(m.contains(Zero{}));
    VERIFY(!m.contains(One{}));
    m.emplace(0, nullptr);
    VERIFY(m.contains(Zero{}));
    VERIFY(!m.contains(One{}));
    m.emplace(1, nullptr);
    VERIFY(m.contains(Zero{}));
    VERIFY(m.contains(One{}));
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "multimap", test01);
ADD_TEST_CASE("", "multimap", test02);
