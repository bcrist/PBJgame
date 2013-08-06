///////////////////////////////////////////////////////////////////////////////
/// \file   be/bed/db.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of be::bed::Db functions.

#include "be/bed/db.h"

#include <cassert>

#include "be/bed/stmt.h"

namespace be {
namespace bed {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs an in-memory database.
Db::Db()
{
   if (sqlite3_open(":memory:", &db_) != SQLITE_OK)
   {
      error e(sqlite3_errmsg(db_));
      sqlite3_close(db_);
      throw e;
   }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Opens the specified database file for read/write access, creating
///         it if it does not exist.
///
/// \param  path The path to the database file.
Db::Db(const std::string& path)
{
   if (sqlite3_open(path.c_str(), &db_) != SQLITE_OK)
   {
      error e(sqlite3_errmsg(db_));
      sqlite3_close(db_);
      throw e;
   }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Opens a database file, using a custom sqlite3_open_v2() flag
///         bitfield.
///
/// \details See SQLite's [Opening A New Database
///         Connection](http://www.sqlite.org/c3ref/open.html) page for more
///         information.
///
/// \param  path The path to the database file.
/// \param  flags A set of sqlite3_open_v2() flags.
Db::Db(const std::string& path, int flags)
{
   if (sqlite3_open_v2(path.c_str(), &db_, flags, nullptr) != SQLITE_OK)
   {
      error e(sqlite3_errmsg(db_));
      sqlite3_close(db_);
      throw e;
   }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Opens a database file, using a custom sqlite3_open_v2() flag
///         bitfield and VFS module.
///
/// \details See SQLite's [Opening A New Database
///         Connection](http://www.sqlite.org/c3ref/open.html) page for more
///         information.
///
/// \param  path The path to the database file.
/// \param  flags A set of sqlite3_open_v2() flags.
/// \param  vfs_name The name of the VFS module to use for low level I/O.
Db::Db(const std::string& path, int flags, const std::string& vfs_name)
{
   if (sqlite3_open_v2(path.c_str(), &db_, flags, vfs_name.c_str()) != SQLITE_OK)
   {
      error e(sqlite3_errmsg(db_));
      sqlite3_close(db_);
      throw e;
   }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Cleans up the Db object.
///
/// \details Assertion failure will result if there are still statement objects
///         attached to this database that have not been destroyed.  It is up
///         to the programmer to ensure that Stmt objects have smaller scope
///         than Db objects.
Db::~Db()
{
   assert(sqlite3_next_stmt(db_, NULL) == NULL);
   int result = sqlite3_close(db_);
   assert(result == SQLITE_OK);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Convienience function for starting a new transaction.
///
/// \sa     Transaction
void Db::begin()
{
   exec("BEGIN");
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Convienience function for comitting a previously started
///         transaction.
///
/// \sa     Transaction
void Db::commit()
{
   exec("END");
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Convienience function for rolling back a previously started
///         transaction.
///
/// \sa     Transaction
void Db::rollback()
{
   exec("ROLLBACK");
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Convienience function for vacuuming the SQLite database.
///
/// \details See SQLite's [VACUUM](http://www.sqlite.org/lang_vacuum.html) page
///         for more information.
void Db::vacuum()
{
   exec("VACUUM");
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compiles & executes one or more SQL queries on the database,
///         discarding any result sets that may be returned.
///
/// \param  sql One or more SQL statements to execute.
void Db::exec(const std::string& sql)
{
   char* err;
   int result = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err);
   if (result != SQLITE_OK)
   {
      if (err)
         throw error(err);
      else
#if defined(_MSC_VER) && _MSC_VER <= 1600
         // VC10 doesn't include std::to_string(int)
         throw error("sqlite3_exec() returned " + std::to_string(static_cast<_Longlong>(result)));
#else
         throw error("sqlite3_exec() returned " + std::to_string(result));
#endif
   }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compiles & executes an SQL query which generates at least one row
///         whose first field is convertible to integer.
///
/// \details If no rows are returned by the query, the default value provided
///         is returned.
///
/// \param  sql The statement to execute.
/// \param  default_value A value to return if there are no rows returned by
//          the SQL statement.
/// \return The first column of the first row of the result set, as an integer,
///         or default_value if no rows are returned.
int Db::getInt(const std::string& sql, int default_value)
{
   Stmt stmt(*this, Id(), sql);
   if (stmt.step())
      return stmt.getInt(0);

   return default_value;
}

} // namespace be::bed
} // namespace be
