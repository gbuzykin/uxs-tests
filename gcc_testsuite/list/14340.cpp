// -*- C++ -*-

// Copyright (C) 2004-2022 Free Software Foundation, Inc.
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

#include <uxs-legacy/list.h>

// NB: This issue affected only debug-mode.

// { dg-do compile }

namespace {

// libstdc++/14340
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
void test01() {
    typedef uxs::list<int> list_type;
    __gnu_test::conversion<list_type>::iterator_to_const_iterator();
}
#pragma GCC diagnostic pop

}  // namespace
