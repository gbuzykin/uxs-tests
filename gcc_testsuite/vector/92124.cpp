// Copyright (C) 2019-2022 Free Software Foundation, Inc.
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

#include "uxs/vector.h"

namespace {

struct X {
    X() = default;
    X(const X&) = default;

    // Move constructor might throw
    X(X&&) noexcept(false) {}

    // Tracking calls to assignment functions
    X& operator=(const X&) { throw 1; }

    X& operator=(X&&) noexcept(true) { return *this; }
};

int test01() {
    using A = __gnu_test::propagating_allocator<X, false>;
    A a1(1), a2(2);
    uxs::vector<X, A> v1(1, a1), v2(1, a2);
    v1 = std::move(v2);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
