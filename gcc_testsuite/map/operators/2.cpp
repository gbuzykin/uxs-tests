// Copyright (C) 2012-2022 Free Software Foundation, Inc.
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

// This test verifies that the value type of a map need not be default copyable.

// { dg-do run { target c++11 } }

#include "gcc_testsuite/util/testsuite_counter_type.h"
#include "gcc_testsuite/util/testsuite_hooks.h"
#include "gcc_testsuite/util/testsuite_rvalref.h"

#include "uxs/map.h"

namespace {

struct Mapped {
    Mapped() = default;
    explicit Mapped(const Mapped&) = default;
};

struct DefaultConstructibleType {
    int val;

    DefaultConstructibleType() : val(123) {}

    DefaultConstructibleType(const DefaultConstructibleType&) = delete;
    DefaultConstructibleType(DefaultConstructibleType&&) = delete;

    DefaultConstructibleType& operator=(int x) {
        val = x;
        return *this;
    }
};

int test01() {
    using __gnu_test::counter_type;
    using __gnu_test::rvalstruct;

    uxs::map<int, Mapped> m1;
    m1[0] = Mapped();

    uxs::map<int, rvalstruct> m2;
    m2[0] = rvalstruct(13);

    uxs::map<int, DefaultConstructibleType> m3;
    VERIFY(m3[0].val == 123);
    VERIFY(m3.size() == 1);
    m3[0] = 2;
    VERIFY(m3[0].val == 2);

    uxs::map<counter_type, int> m4;
    VERIFY(m4[counter_type(1)] == 0);
    VERIFY(counter_type::specialize_count == 1);
    VERIFY(counter_type::copy_count == 0);
    VERIFY(counter_type::move_count == 1);

    counter_type k(2);
    counter_type::reset();

    VERIFY(m4[k] == 0);
    VERIFY(counter_type::copy_count == 1);
    VERIFY(counter_type::move_count == 0);

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "map", test01);
