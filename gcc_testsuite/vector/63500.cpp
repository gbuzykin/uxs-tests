// -*- C++ -*-

// Copyright (C) 2014-2022 Free Software Foundation, Inc.
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

// { dg-do compile { target c++11 } }

#include "uxs/vector.h"

#include <iterator>
#include <memory>

namespace {

class Foo {};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
void test01() {
    uxs::vector<std::unique_ptr<Foo>> v;
    uxs::vector<std::unique_ptr<Foo>> w;

    v.insert(end(v), make_move_iterator(begin(w)), make_move_iterator(end(w)));
}
#pragma GCC diagnostic pop

}  // namespace
