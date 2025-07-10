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

/*
 * The goal with this application is to compare the performance
 * between different std::allocator implementations. The results are
 * influenced by the underlying allocator in the "C" library, malloc.
 */

#include "test_suite.h"

#include <uxs-legacy/set.h>

typedef int test_type;

// This can take extremely long on simulators, timing out the test.
// { dg-options "-DITERATIONS=10" { target simulator } }
#ifndef ITERATIONS
#    define ITERATIONS 10000
#endif

namespace {

// The number of iterations to be performed.
int iterations = ITERATIONS;

// The number of values to insert in the container, 32 will cause 5
// (re)allocations to be performed (sizes 4, 8, 16, 32 and 64)
// This means that all allocations are within _MAX_BYTES = 128 as
// defined in stl_alloc.h for __pool_alloc.  Whether or not this
// value is relevant in "the real world" or not I don't know and
// should probably be investigated in more detail.
int insert_values = 128;

template<typename TestType>
struct value_type : public std::pair<TestType, TestType> {
    value_type() : std::pair<TestType, TestType>(0, 0) {}

    inline value_type operator++() { return ++this->first, *this; }
    inline operator TestType() const { return this->first; }
};

template<typename Container>
void do_loop() {
    Container obj;
    int test_iterations = 0;
    value_type<test_type> test_value;
    while (test_iterations < iterations) {
        for (int j = 0; j < insert_values; ++j) { obj.insert(obj.end(), ++test_value); }
        ++test_iterations;
    }
}

template<typename Container>
void test_container(Container) {
    do_loop<Container>();
}

// http://gcc.gnu.org/ml/libstdc++/2001-05/msg00105.html
// http://gcc.gnu.org/ml/libstdc++/2003-05/msg00231.html
int test01(void) {
    typedef std::less<test_type> compare_type;
    test_container(uxs::set<test_type, compare_type>());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "set", test01);
