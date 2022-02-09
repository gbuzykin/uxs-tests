// Copyright (C) 2015-2022 Free Software Foundation, Inc.
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

#include "gcc_testsuite/util/testsuite_hooks.h"
#include "util/vector.h"

namespace {

template<typename T>
void check(const T& t) {
    for (auto& e : t) VERIFY(!e.moved_from);
}

// This type is CopyInsertable into util::vector<Bomb> so push_back should
// have the strong exception-safety guarantee.
struct Bomb {
    Bomb() = default;

    Bomb(const Bomb& b) : armed(b.armed) { tick(); }

    Bomb(Bomb&& b) noexcept(false) : armed(b.armed) {
        tick();
        b.moved_from = true;
    }

    // util::vector in GCC 4.x tries to use this constructor
    template<typename T>
    Bomb(T&) = delete;

    bool moved_from = false;
    bool armed = true;

 private:
    void tick() {
        if (armed && ticks++) throw 1;
    }

    static int ticks;
};

int Bomb::ticks = 0;

int test01() {
    util::vector<Bomb> v(2);  // fill with armed bombs
    v.resize(v.capacity());   // ensure no unused capacity
    check(v);                 // sanity check

    try {
        Bomb defused;
        // don't want any copies/moves of this object to throw
        defused.armed = false;
        // insert new element, existing elements will be relocated and explode
        v.push_back(defused);
        VERIFY(false);  // should be unreachable
    } catch (int) {
        check(v);  // make sure no elements have been moved from
    }

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);