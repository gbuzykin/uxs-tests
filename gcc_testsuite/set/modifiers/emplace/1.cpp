// { dg-do run { target c++11 } }

// Copyright (C) 2012-2022 Free Software Foundation, Inc.
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

#include <uxs-legacy/set.h>
#include <uxs-legacy/vector.h>

namespace {

class PathPoint {
 public:
    PathPoint(char t, const uxs::vector<double>& c) : type(t), coords(c) {}
    PathPoint(char t, uxs::vector<double>&& c) : type(t), coords(std::move(c)) {}
    char getType() const { return type; }
    const uxs::vector<double>& getCoords() const { return coords; }

 private:
    char type;
    uxs::vector<double> coords;
};

struct PathPointLess {
    bool operator()(const PathPoint& __lhs, const PathPoint& __rhs) const { return __lhs.getType() < __rhs.getType(); }
};

int test01() {
    typedef uxs::set<PathPoint, PathPointLess> Set;
    Set s;

    uxs::vector<double> coord1 = {0.0, 1.0, 2.0};

    auto ret = s.emplace('a', coord1);
    VERIFY(ret.second);
    VERIFY(s.size() == 1);
    VERIFY(ret.first->getType() == 'a');

    coord1[0] = 3.0;
    ret = s.emplace('a', coord1);
    VERIFY(!ret.second);
    VERIFY(s.size() == 1);
    VERIFY(ret.first->getType() == 'a');
    VERIFY(ret.first->getCoords()[0] == 0.0);

    auto it = s.emplace_hint(s.begin(), 'b', coord1);
    VERIFY(it != s.end());
    VERIFY(it->getType() == 'b');
    VERIFY(it->getCoords()[0] == 3.0);

    double* px = &coord1[0];
    ret = s.emplace('c', std::move(coord1));
    VERIFY(ret.second);
    VERIFY(ret.first->getType() == 'c');
    VERIFY(&(ret.first->getCoords()[0]) == px);

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "set", test01);
