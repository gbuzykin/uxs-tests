// { dg-do compile }

// 2000-09-07 bgarcia@laurelnetworks.com

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

// 23.4.4 template class map

#include "test_suite.h"

#include "uxs/map.h"

namespace {

// libstdc++/737
// http://gcc.gnu.org/ml/libstdc++/2000-11/msg00093.html
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
void test01() {
    typedef uxs::map<int, const int> MapInt;

    MapInt m;

    for (unsigned i = 0; i < 10; ++i) { m.insert(MapInt::value_type(i, i)); }

    for (MapInt::const_iterator i = m.begin(); i != m.end(); ++i) {}

    for (MapInt::const_iterator i = m.begin(); m.end() != i; ++i) {}
}
#pragma GCC diagnostic pop

}  // namespace
