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

#include "uxs/vector.h"

namespace {

struct T {
    int i;
};

int swap_calls;

}  // namespace

namespace uxs {
template<>
void vector<T, std::allocator<T>>::swap(vector<T, std::allocator<T>>&) noexcept {
    ++swap_calls;
}
}  // namespace uxs

namespace {

// Should use vector specialization for swap.
int test01() {
    uxs::vector<T> A;
    uxs::vector<T> B;
    swap_calls = 0;
    std::swap(A, B);
    VERIFY(1 == swap_calls);
    return 0;
}

// Should use vector specialization for swap.
int test02() {
    using namespace std;
    uxs::vector<T> A;
    uxs::vector<T> B;
    swap_calls = 0;
    swap(A, B);
    VERIFY(1 == swap_calls);
    return 0;
}

}  // namespace

// See c++/13658 for background info.
ADD_TEST_CASE("", "vector", test01);
ADD_TEST_CASE("", "vector", test02);
