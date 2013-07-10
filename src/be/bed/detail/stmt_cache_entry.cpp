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
/// \file   be/bed/detail/stmt_cache_entry.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of be::bed::detail::StmtCacheEntry functions.

#include "be/bed/detail/stmt_cache_entry.h"

namespace be {
namespace bed {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Default constructor.
///
/// \details Constructs an entry in an invalid state.  stmt must still be
///         initialized before the object is usable.
StmtCacheEntry::StmtCacheEntry()
   : held(false),
     access_index(0)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new entry which owns the statement object provided.
///
/// \details The Stmt provided must have been created using new Stmt(...).
///
/// \param  stmt The statement object used for this entry.
StmtCacheEntry::StmtCacheEntry(Stmt* stmt)
   : held(false),
     access_index(0),
     stmt(stmt)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Move-constructs an existing entry.
///
/// \param  other An existing entry to move from.
StmtCacheEntry::StmtCacheEntry(StmtCacheEntry&& other)
   : held(std::move(other.held)),
     access_index(std::move(other.access_index)),
     stmt(std::move(other.stmt))
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Move-assigns an existing entry.
///
/// \param  other An existing entry to move from.
void StmtCacheEntry::operator=(StmtCacheEntry&& other)
{
   held = std::move(other.held);
   access_index = std::move(other.access_index);
   stmt = std::move(other.stmt);
}

} // namespace be::bed::detail
} // namespace be::bed
} // namespace be
