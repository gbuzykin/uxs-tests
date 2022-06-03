// { dg-do run { target c++11 } }

// 2007-10-28  Paolo Carlini  <pcarlini@suse.de>

// Copyright (C) 2007-2022 Free Software Foundation, Inc.
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

#include "uxs/vector.h"

#include "gcc_testsuite/util/testsuite_hooks.h"
#include "gcc_testsuite/util/testsuite_rvalref.h"

namespace {

int test01() {
    using namespace __gnu_test;

    uxs::vector<copycounter> a(40);
    copycounter::copycount = 0;

    a.reserve(50);
    VERIFY(copycounter::copycount == 0);

    a.reserve(200);
    VERIFY(copycounter::copycount == 0);

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
