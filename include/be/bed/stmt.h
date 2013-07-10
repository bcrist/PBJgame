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
/// \file   be/bed/stmt.h
/// \author Benjamin Crist
///
/// \brief  be::bed::Stmt class header.

#ifndef BE_BED_STMT_H_
#define BE_BED_STMT_H_
#include "be/_be.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <cassert>
#include <ostream>
#include "sqlite3.h"

#include "be/bed/db.h"
#include "be/id.h"

namespace be {
namespace bed {

///////////////////////////////////////////////////////////////////////////////
/// \class  Stmt   be/bed/stmt.h "be/bed/stmt.h"
///
/// \brief  RAII wrapper for SQLite's sqlite3_stmt API.
/// \details Stmt objects are non-copyable and non-moveable and represent a
///         prepared statement -- a compiled SQL query -- along with its
///         currently bound parameters and the resultset returned when the
///         query is executed.
/// \ingroup db
class Stmt
{
public:
   Stmt(Db& db, const std::string& sql);
   Stmt(Db& db, const Id& id, const std::string& sql);
   ~Stmt();

   Db& getDb();

   const Id& getId() const;

   const char* getSql() const;

   int parameters();
   int parameter(const std::string& name);

   void bind();
   void bind(int parameter);
   void bind(int parameter, bool value);
   void bind(int parameter, int value);
   void bind(int parameter, unsigned int value);
   void bind(int parameter, sqlite3_int64 value);
   void bind(int parameter, sqlite3_uint64 value);
   void bind(int parameter, double value);
   void bind(int parameter, const std::string& value);
   void bind(int parameter, const char* value);
   void bind_s(int parameter, const char* value);
   void bindBlob(int parameter, const std::string& value);
   void bindBlob(int parameter, const void* value, int length);
   void bindBlob_s(int parameter, const void* value, int length);

   bool step();
   void reset();

   int columns();
   int column(const std::string& name);

   int getType(int column);

   bool getBool(int column);
   int getInt(int column);
   unsigned int getUInt(int column);
   sqlite3_int64 getInt64(int column);
   sqlite3_uint64 getUInt64(int column);
   double getDouble(int column);
   const char* getText(int column);
   std::string getBlob(int column);
   int getBlob(int column, const void*& dest);

private:
   Db& db_;
   Id id_;
   sqlite3_stmt* stmt_;
   std::unique_ptr<std::unordered_map<std::string, int> > col_names_;

   Stmt(const Stmt&);
   void operator=(const Stmt&);
};

std::ostream& operator<<(std::ostream& os, const Stmt& s);

} // namespace be::bed
} // namespace be

#endif
