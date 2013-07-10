// Copyright (c) 2013 Benjamin Crist
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
/// \file   be/bed/detail/stmt_cache_entry.h
/// \author Benjamin Crist
///
/// \brief  be::bed::detail::StmtCacheEntry class header.

#ifndef BE_BED_DETAIL_STMT_CACHE_ENTRY_H_
#define BE_BED_DETAIL_STMT_CACHE_ENTRY_H_

#include "be/bed/stmt.h"

#include <memory>

namespace be {
namespace bed {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
/// \class  StmtCacheEntry   be/bed/detail/stmt_cache_entry.h "be/bed/detail/stmt_cache_entry.h"
///
/// \brief  Used by StmtCache to keep track of which statements are currently
///         held or unheld, and how recently each statement was held.
struct StmtCacheEntry
{
public:
   StmtCacheEntry();
   StmtCacheEntry(Stmt* stmt);

   StmtCacheEntry(StmtCacheEntry&& other);
   void operator=(StmtCacheEntry&& other);

   bool held;
   size_t access_index;
   std::unique_ptr<Stmt> stmt;

private:
   StmtCacheEntry(const StmtCacheEntry&);
   void operator=(const StmtCacheEntry&);
};

} // namespace be::bed::detail
} // namespace be::bed
} // namespace be

#endif
