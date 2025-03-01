// { dg-do compile }

// Copyright (C) 2007-2022 Free Software Foundation, Inc.
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

#include "uxs/map.h"

namespace {

// libstdc++/31440
struct DagNode {};

class MemoTable {
 public:
    void memoRewrite();

 private:
    struct dagNodeLt;
    class MemoMap;

    MemoMap* memoMap;
};

struct MemoTable::dagNodeLt {
    bool operator()(const DagNode*, const DagNode*) const { return false; }
};

class MemoTable::MemoMap : public uxs::map<DagNode*, DagNode*, MemoTable::dagNodeLt> {};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
void MemoTable::memoRewrite() { memoMap->find(0); }
#pragma GCC diagnostic pop

}  // namespace
