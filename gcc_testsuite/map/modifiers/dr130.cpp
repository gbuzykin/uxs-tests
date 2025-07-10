// { dg-do run { target c++11 } }
// 2008-07-22  Edward Smith-Rowland  <3dw4rd@verizon.net>
//
// Copyright (C) 2009-2022 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include "gcc_testsuite/util/testsuite_hooks.h"

#include <uxs-legacy/map.h>

namespace {

//  DR 130. Associative erase should return an iterator.
int test01() {
    uxs::map<int, int> m0;
    typedef uxs::map<int, int>::iterator iterator;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
    typedef uxs::map<int, int>::const_iterator const_iterator;
#pragma GCC diagnostic pop
    typedef uxs::map<int, int>::value_type value_type;
    typedef std::pair<iterator, bool> insert_return_type;

    m0.insert(value_type(1, 1));
    insert_return_type irt1 = m0.insert(value_type(2, 2));
    insert_return_type irt2 = m0.insert(value_type(3, 3));

    iterator pos1 = m0.erase(irt1.first);
    VERIFY(pos1 == irt2.first);

    iterator pos2 = m0.erase(irt2.first);
    VERIFY(pos2 == m0.end());
    return 0;
}

int test02() {
    uxs::map<int, int> m0;
    typedef uxs::map<int, int>::iterator iterator;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
    typedef uxs::map<int, int>::const_iterator const_iterator;
#pragma GCC diagnostic pop
    typedef uxs::map<int, int>::value_type value_type;
    typedef std::pair<iterator, bool> insert_return_type;

    insert_return_type irt0 = m0.insert(value_type(1, 1));
    m0.insert(value_type(2, 2));
    insert_return_type irt2 = m0.insert(value_type(3, 3));
    insert_return_type irt3 = m0.insert(value_type(4, 4));

    iterator pos1 = m0.erase(irt0.first, irt2.first);
    VERIFY(pos1 == irt2.first);

    iterator pos2 = m0.erase(irt2.first, ++irt3.first);
    VERIFY(pos2 == m0.end());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "map", test01);
ADD_TEST_CASE("", "map", test02);
