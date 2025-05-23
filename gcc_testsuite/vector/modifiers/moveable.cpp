// { dg-do run { target c++11 } }

// Copyright (C) 2005-2022 Free Software Foundation, Inc.
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

#include "gcc_testsuite/util/testsuite_hooks.h"
#include "gcc_testsuite/util/testsuite_rvalref.h"

#include "uxs/vector.h"

using namespace __gnu_test;

namespace {

// Test vector::push_back makes no unneeded copies.
int test01() {
    uxs::vector<copycounter> a;
    copycounter c(1);
    copycounter::copycount = 0;
    for (int i = 0; i < 10; ++i) { a.push_back(c); }
    VERIFY(copycounter::copycount == 10);

    for (int i = 0; i < 100; ++i) { a.insert(a.begin() + i, c); }
    VERIFY(copycounter::copycount == 110);

    for (int i = 0; i < 1000; ++i) { a.insert(a.end(), c); }
    VERIFY(copycounter::copycount == 1110);
    return 0;
}

// Test vector::insert(iterator, iterator, iterator) makes no unneeded copies
// when it has to also reallocate the vector's internal buffer.
int test02() {
    copycounter c(1);
    uxs::vector<copycounter> a(10, c), b(100, c);
    copycounter::copycount = 0;
    a.insert(a.begin(), b.begin(), b.begin() + 20);
    VERIFY(copycounter::copycount == 20);
    a.insert(a.end(), b.begin(), b.begin() + 50);
    VERIFY(copycounter::copycount == 70);
    a.insert(a.begin() + 50, b.begin(), b.end());
    VERIFY(copycounter::copycount == 170);
    return 0;
}

// Test vector::insert(iterator, iterator, iterator) makes no unneeded copies
// when it doesn't have to reallocate the vector's internal buffer.
int test03() {
    copycounter c(1);
    uxs::vector<copycounter> a(10, c), b(100, c);
    copycounter::copycount = 0;
    a.reserve(1000);
    VERIFY(copycounter::copycount == 0);
    a.insert(a.begin(), b.begin(), b.begin() + 20);
    VERIFY(copycounter::copycount == 20);
    a.insert(a.end(), b.begin(), b.begin() + 50);
    VERIFY(copycounter::copycount == 70);
    a.insert(a.begin() + 50, b.begin(), b.end());
    VERIFY(copycounter::copycount == 170);
    return 0;
}

// Test vector::insert(iterator, count, value) makes no unneeded copies
// when it has to also reallocate the vector's internal buffer.
int test04() {
    copycounter c(1);
    uxs::vector<copycounter> a(10, c);
    copycounter::copycount = 0;
    a.insert(a.begin(), 20, c);
    VERIFY(copycounter::copycount == 20);
    a.insert(a.end(), 50, c);
    VERIFY(copycounter::copycount == 70);
    a.insert(a.begin() + 50, 100, c);
    VERIFY(copycounter::copycount == 170);
    return 0;
}

// Test vector::insert(iterator, count, value) makes no unneeded copies
// when it doesn't have to reallocate the vector's internal buffer.
int test05() {
    copycounter c(1);
    uxs::vector<copycounter> a(10, c);
    copycounter::copycount = 0;
    a.reserve(1000);
    a.insert(a.begin(), 20, c);
    // NOTE : These values are each one higher than might be expected, as
    // vector::insert(iterator, count, value) copies the value it is given
    // when it doesn't reallocate the buffer.
    VERIFY(copycounter::copycount == 20 + 1);
    a.insert(a.end(), 50, c);
    VERIFY(copycounter::copycount == 70 + 2);
    a.insert(a.begin() + 50, 100, c);
    VERIFY(copycounter::copycount == 170 + 3);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
ADD_TEST_CASE("", "vector", test02);
ADD_TEST_CASE("", "vector", test03);
ADD_TEST_CASE("", "vector", test04);
ADD_TEST_CASE("", "vector", test05);
