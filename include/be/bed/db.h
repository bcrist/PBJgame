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
/// \file   be/bed/db.h
/// \author Benjamin Crist
///
/// \brief  be::bed::Db class header.

#ifndef BE_BED_DB_H_
#define BE_BED_DB_H_
#include "be/_be.h"

#include <string>
#include "sqlite3.h"

#include "be/bed/detail/db_error.h"
#include "be/id.h"

namespace be {
namespace bed {

class Stmt;

///////////////////////////////////////////////////////////////////////////////
/// \class  Db   be/bed/db.h "be/bed/db.h"
///
/// \brief  RAII wrapper for SQLite's sqlite3 API.
/// \details DB objects are non-copyable and non-moveable and represent an open
///         connection with an SQLite database file.
/// \ingroup db
class Db
{
   friend class Stmt;
public:
   typedef detail::db_error error; ///< Exception type thrown when an SQLite function fails.  If related to a SQL query, it can be retrieved using Db::error::sql().

   Db();
   explicit Db(const std::string& path);
   explicit Db(const std::string& path, int flags);
   explicit Db(const std::string& path, int flags, const std::string& vfs_name);
   ~Db();

   void begin();
   void commit();
   void rollback();

   void vacuum();

   void exec(const std::string& sql);

   int getInt(const std::string& sql, int default_value);

private:
   sqlite3* db_;

   Db(const Db&);
   void operator=(const Db&);
};

} // namespace be::bed
} // namespace be

#endif
