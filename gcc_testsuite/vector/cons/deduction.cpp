// Copyright (C) 2017-2022 Free Software Foundation, Inc.
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

// { dg-do compile { target c++17 } }

#if __cplusplus >= 201703L

#    include "gcc_testsuite/util/testsuite_allocators.h"
#    include "gcc_testsuite/util/testsuite_iterators.h"
#    include "util/vector.h"

namespace {

template<typename T>
using input_iterator_seq = __gnu_test::test_container<T, __gnu_test::input_iterator_wrapper>;

template<typename T, typename U>
struct require_same;
template<typename T>
struct require_same<T, T> {
    using type = void;
};

template<typename T, typename U>
typename require_same<T, U>::type check_type(U&) {}

struct Pool;

template<typename T>
struct Alloc : __gnu_test::SimpleAllocator<T> {
    Alloc(Pool*) {}

    template<typename U>
    Alloc(const Alloc<U>&) {}
};

int test01() {
    util::vector<unsigned> s0;

    util::vector s1 = s0;
    check_type<util::vector<unsigned>>(s1);

    util::vector s2 = std::move(s0);
    check_type<util::vector<unsigned>>(s2);

    const util::vector s3 = s0;
    check_type<const util::vector<unsigned>>(s3);

    const util::vector s4 = s3;
    check_type<const util::vector<unsigned>>(s4);

    return 0;
}

int test02() {
    unsigned a[1] = {};
    input_iterator_seq<unsigned> seq(a);

    util::vector s1(seq.begin(), seq.end());
    check_type<util::vector<unsigned>>(s1);

    util::vector s2(seq.begin(), seq.end(), std::allocator<unsigned>());
    check_type<util::vector<unsigned>>(s2);

    util::vector s3(1U, 2L);
    check_type<util::vector<long>>(s3);

    util::vector s4(1U, 2L, std::allocator<long>());
    check_type<util::vector<long>>(s4);

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
ADD_TEST_CASE("", "vector", test02);

#endif
