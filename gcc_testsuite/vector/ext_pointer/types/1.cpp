// Test for Container using non-standard pointer types.

// Copyright (C) 2008-2022 Free Software Foundation, Inc.
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

// This is a copy of vector/types/1.cc with altered allocator.
// The operator+()s in this test initially failed the test -
// they stress the accurate recognition, by the compiler,
// of _Pointer_adapter's own pointer arithmetic functions,
// which have to match perfectly on the int type to get
// chosen by the compiler when it sees: _Pointer_adapter<T> + int, etc.

#if !defined(__clang__)

#    include "test_suite.h"
#    include "util/vector.h"

#    include <ext/extptr_allocator.h>

namespace N {
struct X {};

template<typename T>
X operator+(T, std::size_t) {
    return X();
}

template<typename T>
X operator-(T, T) {
    return X();
}
}  // namespace N

namespace {

int test01() {
    util::vector<N::X, __gnu_cxx::_ExtPtr_allocator<N::X>> v(5);
    const util::vector<N::X, __gnu_cxx::_ExtPtr_allocator<N::X>> w(1);

    (void)v[0];
    (void)w[0];
    (void)v.size();
    (void)v.capacity();
    v.resize(1);
    v.insert(v.begin(), N::X());
    v.insert(v.begin(), 1, N::X());
    v.insert(v.begin(), w.begin(), w.end());
    v = w;

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);

#endif  // !defined(__clang__)
