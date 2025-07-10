// { dg-do run { target c++11 } }

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

#include "gcc_testsuite/util/testsuite_hooks.h"

#include <uxs-legacy/vector.h>

namespace {

struct A {
    A(int i) : _i(i) {}

    A(const A& other) : _i(other._i) { VERIFY(other._i >= 0); }

    A(A&& other) : _i(other._i) {
        VERIFY(other._i >= 0);

        other._i = -1;
    }

    A(uxs::vector<A>::iterator it) : _i(it->_i) { VERIFY(it->_i >= 0); }

    A& operator=(const A&) = default;
    A& operator=(A&& other) {
        VERIFY(other._i >= 0);

        _i = other._i;
        other._i = -1;
        return *this;
    }

    int _i;
};

int test01() {
    uxs::vector<uxs::vector<int>> vv = {{2, 3}, {4, 5}, {0, 1}};

    // Make sure emplace will imply reallocation.
    VERIFY(vv.capacity() == 3);

    vv.emplace(vv.begin(), vv[0]);

    VERIFY(vv.size() == 4);
    VERIFY(vv[0].size() == 2);
    VERIFY(vv[0][0] == 2);
    VERIFY(vv[0][1] == 3);

    return 0;
}

int test02() {
    uxs::vector<uxs::vector<int>> vv = {{2, 3}, {4, 5}, {0, 1}};

    // Make sure emplace won't reallocate.
    vv.reserve(4);
    vv.emplace(vv.begin(), vv[0]);

    VERIFY(vv.size() == 4);
    VERIFY(vv[0].size() == 2);
    VERIFY(vv[0][0] == 2);
    VERIFY(vv[0][1] == 3);

    return 0;
}

int test03() {
    uxs::vector<A> va = {{A(1)}, {A(2)}, {A(3)}};

    // Make sure emplace will imply reallocation.
    VERIFY(va.capacity() == 3);

    va.emplace(va.begin(), va.begin());

    VERIFY(va.size() == 4);
    VERIFY(va[0]._i == 1);

    return 0;
}

int test04() {
    uxs::vector<A> va = {{A(1)}, {A(2)}, {A(3)}};

    // Make sure emplace won't reallocate.
    va.reserve(4);
    va.emplace(va.begin(), va.begin());

    VERIFY(va.size() == 4);
    VERIFY(va[0]._i == 1);

    return 0;
}

int test05() {
    // LWG DR 2164
    uxs::vector<int> v;
    v.reserve(4);
    v = {1, 2, 3};
    v.emplace(v.begin(), v.back());
    VERIFY(v[0] == 3);
    VERIFY(v[1] == 1);
    VERIFY(v[2] == 2);
    VERIFY(v[3] == 3);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
ADD_TEST_CASE("", "vector", test02);
ADD_TEST_CASE("", "vector", test03);
ADD_TEST_CASE("", "vector", test04);
ADD_TEST_CASE("", "vector", test05);
