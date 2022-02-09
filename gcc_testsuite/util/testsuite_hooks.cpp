// -*- C++ -*-

// Utility subroutines for the C++ library testsuite.
//
// Copyright (C) 2002-2022 Free Software Foundation, Inc.
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
//

#include "gcc_testsuite/util/testsuite_hooks.h"

namespace __gnu_test {

object_counter::size_type object_counter::count = 0;
unsigned int copy_constructor::count_ = 0;
unsigned int copy_constructor::throw_on_ = 0;
unsigned int assignment_operator::count_ = 0;
unsigned int assignment_operator::throw_on_ = 0;
unsigned int destructor::_M_count = 0;
int copy_tracker::next_id_ = 0;

}  // namespace __gnu_test
