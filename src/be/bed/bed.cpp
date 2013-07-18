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
/// \file   be/bed/bed.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of be::bed::Bed functions.

#include "be/bed/bed.h"

namespace be {
namespace bed {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a bed using the provided parameters.
///
/// \param  id The Id of the bed.
/// \param  path The location of the bed's SQLite database.
/// \param  read_only Whether or not the bed should be modifiable.
Bed::Bed(const Id& id, const std::string& path, bool read_only)
   : id_(id),
     db_(path, read_only ? SQLITE_OPEN_READONLY : SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE),
     stmt_cache_(db_)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves this bed's ID (usually hashed from the database path).
///
/// \return The bed's Id.
const Id& Bed::getId() const
{
   return id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the Db object representing the connection to this bed's
///         database file.
///
/// \return The Db object associated with the bed.
Db& Bed::getDb()
{
   return db_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a StmtCache object which can be used for executing
///         statements and queries on this bed's database.
///
/// \return The bed's StmtCache.
StmtCache& Bed::getStmtCache()
{
   return stmt_cache_;
}

} // namespace bed
} // namespace bengine
