// 1999-06-24 bkoz

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

// 23.3.4 template class multiset

#include "gcc_testsuite/util/testsuite_hooks.h"

#include "uxs/multiset.h"

#include <algorithm>
#include <iterator>
#include <sstream>

namespace std {
std::ostream& operator<<(std::ostream& os, const std::pair<int, int>& p) { return os << p.first << ' ' << p.second; }
}  // namespace std

bool operator<(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) { return lhs.first < rhs.first; }

namespace {

int test01() {
    typedef uxs::multiset<std::pair<int, int>>::iterator iterator;
    std::pair<int, int> p(69, 0);
    uxs::multiset<std::pair<int, int>> s;

    for (p.second = 0; p.second < 5; ++p.second) { s.insert(p); }
    for (iterator it = s.begin(); it != s.end(); ++it) {
        if (it->second < 5) {
            s.insert(it, p);
            ++p.second;
        }
    }

    std::ostringstream stream;
    std::copy(s.begin(), s.end(), std::ostream_iterator<std::pair<int, int>>(stream, "\n"));
    const std::string expected(
        "69 0\n69 1\n69 2\n69 3\n69 4\n"
        "69 5\n69 6\n69 7\n69 8\n69 9\n");
    std::string tested(stream.str());
    VERIFY(tested == expected);

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "multiset", test01);
