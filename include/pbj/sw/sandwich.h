// Copyright (c) 2013 PBJ^2 Productions
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/sw/sandwich.h
/// \author Benjamin Crist
///
/// \brief  pbj::sw::Sandwich class header.

#ifndef PBJ_SW_SANDWICH_H_
#define PBJ_SW_SANDWICH_H_

#include "be/bed/db.h"
#include "be/bed/stmt.h"
#include "be/bed/stmt_cache.h"
#include "be/id.h"
#include "be/source_handle.h"
#include "pbj/_pbj.h"

#include <memory>

namespace pbj {
namespace sw {

///////////////////////////////////////////////////////////////////////////////
/// \class  Sandwich   pbj/sw/sandwich.h "pbj/sw/sandwich.h"
///
/// \brief  Represents a database file which can be used to access persistent
///         game data.
class Sandwich : public std::enable_shared_from_this<Sandwich>
{
public:
   Sandwich(const std::string& path, bool read_only);

   const Id& getId() const;

   db::Db& getDb();
   db::StmtCache& getStmtCache();

private:
   Id id_;
   db::Db db_;
   db::StmtCache stmt_cache_;

   Sandwich(const Sandwich&);
   void operator=(const Sandwich&);
};

} // namespace be::bed
} // namespace be

#endif
