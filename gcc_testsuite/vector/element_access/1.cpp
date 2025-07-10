// 2000-09-06
// bkoz

// Copyright (C) 2000-2022 Free Software Foundation, Inc.
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

// 23.2.4 vector

#include "gcc_testsuite/util/testsuite_hooks.h"

#include <uxs-legacy/vector.h>

namespace {

template<typename T>
struct A {};

struct B {};

// http://gcc.gnu.org/ml/libstdc++/2000-09/msg00002.html
int test01() {
    uxs::vector<A<B>> vec01;
    uxs::vector<A<B>> vec02(5);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
    typedef uxs::vector<A<B>>::size_type size_type;
#pragma GCC diagnostic pop
    typedef uxs::vector<A<B>>::reference reference;

    try {
        reference r01 __attribute__((unused)) = vec01.at(6);
        VERIFY(false);  // Should not get here, as exception thrown.
    } catch (std::out_of_range& err) { VERIFY(true); } catch (...) {
        VERIFY(false);
    }

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
