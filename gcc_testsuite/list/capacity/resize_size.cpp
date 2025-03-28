// { dg-do run { target c++11 } }

// 2010-06-18  Paolo Carlini  <paolo.carlini@oracle.com>

// Copyright (C) 2010-2022 Free Software Foundation, Inc.
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

#include "gcc_testsuite/util/testsuite_api.h"
#include "gcc_testsuite/util/testsuite_hooks.h"

#include "uxs/list.h"

namespace {

int test01() {
    uxs::list<__gnu_test::NonCopyConstructible> l;
    VERIFY(std::distance(l.begin(), l.end()) == 0);

    l.resize(1000);
    VERIFY(std::distance(l.begin(), l.end()) == 1000);
    for (auto it = l.begin(); it != l.end(); ++it) { VERIFY(*it == -1); }
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test01);
