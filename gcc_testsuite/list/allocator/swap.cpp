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

#include "gcc_testsuite/util/testsuite_allocator.h"
#include "gcc_testsuite/util/testsuite_hooks.h"

#include <uxs-legacy/list.h>

using __gnu_test::propagating_allocator;

namespace {

struct T {
    int i;
};

// It is undefined behaviour to swap() containers wth unequal allocators
// if the allocator doesn't propagate, so ensure the allocators compare
// equal, while still being able to test propagation via get_personality().
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
bool operator==(const propagating_allocator<T, false>&, const propagating_allocator<T, false>&) { return true; }
bool operator!=(const propagating_allocator<T, false>&, const propagating_allocator<T, false>&) { return false; }
#pragma GCC diagnostic pop

int test01() {
    typedef propagating_allocator<T, false> alloc_type;
    typedef uxs::list<T, alloc_type> test_type;
    test_type v1(alloc_type(1));
    v1.push_front(T());
    test_type v2(alloc_type(2));
    v2.push_front(T());
    std::swap(v1, v2);
    VERIFY(1 == v1.get_allocator().get_personality());
    VERIFY(2 == v2.get_allocator().get_personality());
    // swap back so assertions in uneq_allocator::deallocate don't fail
    std::swap(v1, v2);
    return 0;
}

int test02() {
    typedef propagating_allocator<T, true> alloc_type;
    typedef uxs::list<T, alloc_type> test_type;
    test_type v1(alloc_type(1));
    v1.push_front(T());
    test_type v2(alloc_type(2));
    v2.push_front(T());
    std::swap(v1, v2);
    VERIFY(2 == v1.get_allocator().get_personality());
    VERIFY(1 == v2.get_allocator().get_personality());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test01);
ADD_TEST_CASE("", "list", test02);
