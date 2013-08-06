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
