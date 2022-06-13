// 1999-05-07
// bkoz

// Copyright (C) 1999-2022 Free Software Foundation, Inc.
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

// 23.2.4.2 vector capacity

#include "gcc_testsuite/util/testsuite_allocator.h"
#include "gcc_testsuite/util/testsuite_hooks.h"

#include "uxs/vector.h"

#include <stdexcept>

namespace {

// libstdc++/8230
int test02() {
    bool test = true;
#ifdef NDEBUG  // ASAN bad alloc error
    {
        uxs::vector<int> array;
        const std::size_t size = array.max_size();
        try {
            array.reserve(size);
        } catch (const std::length_error& error) { test &= false; } catch (const std::bad_alloc& error) {
            test &= true;
        } catch (...) { test &= false; }
        VERIFY(test);
    }
#endif  // NDEBUG

    {
        uxs::vector<int> array;
        const std::size_t size = array.max_size() + 1;
        try {
            array.reserve(size);
        } catch (const std::length_error& error) { test &= true; } catch (...) {
            test &= false;
        }
        VERIFY(test);
    }

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test02);
