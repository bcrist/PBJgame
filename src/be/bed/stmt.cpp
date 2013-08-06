///////////////////////////////////////////////////////////////////////////////
/// \file   be/bed/stmt.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of be::bed::Stmt functions.

#include "be/bed/stmt.h"

namespace be {
namespace bed {
   
///////////////////////////////////////////////////////////////////////////////
/// \brief  Compiles the provided SQL query against the provided database and
///         constructs a Stmt object to represent it.
///
/// \details A statement Id will be generated automatically by hashing the
///         SQL text provided.
///
/// \param  db The database on which the statement is to be executed.
/// \param  sql The SQL text of the statement to compile.
Stmt::Stmt(Db& db, const std::string &sql)
   : db_(db),
     id_(sql)
{
   if (sqlite3_prepare_v2(db.db_, sql.c_str(), sql.length() + 1, &stmt_, nullptr) != SQLITE_OK)
      throw Db::error(sqlite3_errmsg(db.db_), sql.c_str());
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compiles the provided SQL query against the provided database and
///         constructs a Stmt object to represent it using the provided ID.
///
/// \param  db The database on which the statement is to be executed.
/// \param  id The Id of the statement.
/// \param  sql The SQL text of the statement to compile.
Stmt::Stmt(Db& db, const Id& id, const std::string &sql)
   : db_(db),
     id_(id)
{
   if (sqlite3_prepare_v2(db.db_, sql.c_str(), sql.length() + 1, &stmt_, nullptr) != SQLITE_OK)
      throw Db::error(sqlite3_errmsg(db.db_), sql.c_str());
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destroys a Stmt object.
///
/// \details All Stmt objects must be destroyed before their associated Db
///         object can be destroyed.  Not doing so is a programming error, and
///         may result in an assertion failure.
Stmt::~Stmt()
{
   sqlite3_finalize(stmt_);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the DB object associated with this prepared statement.
///
/// \return The DB object associated with this prepared statement.
Db& Stmt::getDb()
{
   return db_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns this statement's Id.
///
/// \return The Id of this statement (usually hashed from the SQL text).
const Id& Stmt::getId() const
{
   return id_;
}


///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the uncompiled text of the SQL query.
///
/// \return The uncompiled SQL query text, as a c-string.
const char* Stmt::getSql() const
{
   return sqlite3_sql(stmt_);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the number of bindable parameters present in the query.
///
/// \details Parameter indices start at 1, so this is actually the largest
///         valid parameter index which can be used.
///
/// \return The index of the highest bindable parameter in the query.
int Stmt::parameters()
{
   return sqlite3_bind_parameter_count(stmt_);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the index of a named parameter in the query.
///
/// \param  name The parameter name specified in the SQL text.
/// \return The parameter index corresponding to the requested parameter name.
int Stmt::parameter(const std::string& name)
{
   int result = sqlite3_bind_parameter_index(stmt_, name.c_str());
   assert(result != 0);
   return result;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Clears all parameter bindings.
///
/// \details Binds null to each parameter index.
void Stmt::bind()
{
   if (sqlite3_clear_bindings(stmt_) != SQLITE_OK)
      throw Db::error(sqlite3_errmsg(db_.db_), sqlite3_sql(stmt_));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Clears the specified parameter binding.
///
/// \param  parameter The index of the parameter to which null will be bound.
void Stmt::bind(int parameter)
{
   if (sqlite3_bind_null(stmt_, parameter) != SQLITE_OK)
      throw Db::error(sqlite3_errmsg(db_.db_), sqlite3_sql(stmt_));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Binds a bool value to the specified parameter.
///
/// \details Since SQLite doesn't directly support boolean values, true is
///         represented as an integral 1, while false is represented as zero.
///
/// \param  parameter The index of the parameter to bind.
/// \param  value The value to bind to the parameter.
void Stmt::bind(int parameter, bool value)
{
   if (sqlite3_bind_int(stmt_, parameter, value ? 1 : 0) != SQLITE_OK)
      throw Db::error(sqlite3_errmsg(db_.db_), sqlite3_sql(stmt_));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Binds an integer value to the specified parameter.
///
/// \param  parameter The index of the parameter to bind.
/// \param  value The value to bind to the parameter.
void Stmt::bind(int parameter, int value)
{
   if (sqlite3_bind_int(stmt_, parameter, value) != SQLITE_OK)
      throw Db::error(sqlite3_errmsg(db_.db_), sqlite3_sql(stmt_));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Binds an unsigned integer value to the specified parameter.
///
/// \param  parameter The index of the parameter to bind.
/// \param  value The value to bind to the parameter.
void Stmt::bind(int parameter, unsigned int value)
{
   if (sqlite3_bind_int64(stmt_, parameter, static_cast<sqlite3_int64>(value)) != SQLITE_OK)
      throw Db::error(sqlite3_errmsg(db_.db_), sqlite3_sql(stmt_));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Binds a 64-bit integer value to the specified parameter.
///
/// \param  parameter The index of the parameter to bind.
/// \param  value The value to bind to the parameter.
void Stmt::bind(int parameter, sqlite3_int64 value)
{
   if (sqlite3_bind_int64(stmt_, parameter, value) != SQLITE_OK)
      throw Db::error(sqlite3_errmsg(db_.db_), sqlite3_sql(stmt_));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Binds a unsigned 64-bit integer value to the specified parameter.
///
/// \details Values are cast to signed integers before being stored, so very
///         large integers may be stored as negative numbers.
///
/// \param  parameter The index of the parameter to bind.
/// \param  value The value to bind to the parameter.
void Stmt::bind(int parameter, sqlite3_uint64 value)
{
   if (sqlite3_bind_int64(stmt_, parameter, static_cast<sqlite3_int64>(value)) != SQLITE_OK)
      throw Db::error(sqlite3_errmsg(db_.db_), sqlite3_sql(stmt_));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Binds a 64-bit floating point value to the specified parameter.
///
/// \param  parameter The index of the parameter to bind.
/// \param  value The value to bind to the parameter.
void Stmt::bind(int parameter, double value)
{
   if (sqlite3_bind_double(stmt_, parameter, value) != SQLITE_OK)
      throw Db::error(sqlite3_errmsg(db_.db_), sqlite3_sql(stmt_));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Binds a text string value to the specified parameter.
///
/// \param  parameter The index of the parameter to bind.
/// \param  value The value to bind to the parameter.
void Stmt::bind(int parameter, const std::string& value)
{
   if (sqlite3_bind_text(stmt_, parameter, value.c_str(), value.length(), SQLITE_TRANSIENT) != SQLITE_OK)
      throw Db::error(sqlite3_errmsg(db_.db_), sqlite3_sql(stmt_));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Binds a text string value to the specified parameter.
///
/// \param  parameter The index of the parameter to bind.
/// \param  value The value to bind to the parameter.
void Stmt::bind(int parameter, const char* value)
{
   if (sqlite3_bind_text(stmt_, parameter, value, -1, SQLITE_TRANSIENT) != SQLITE_OK)
      throw Db::error(sqlite3_errmsg(db_.db_), sqlite3_sql(stmt_));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Binds a static text string value to the specified parameter.
///
/// \details The parameter must be located in static memory, since it may be
///         accessed at a later point.  For instance, this should be used over
///         bind(int, const char*) when the value is a string literal.
///
/// \param  parameter The index of the parameter to bind.
/// \param  value The value to bind to the parameter.
void Stmt::bind_s(int parameter, const char* value)
{
   if (sqlite3_bind_text(stmt_, parameter, value, -1, SQLITE_STATIC) != SQLITE_OK)
      throw Db::error(sqlite3_errmsg(db_.db_), sqlite3_sql(stmt_));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Binds a binary string value to the specified parameter.
///
/// \param  parameter The index of the parameter to bind.
/// \param  value The value to bind to the parameter.
void Stmt::bindBlob(int parameter, const std::string& value)
{
   if (sqlite3_bind_blob(stmt_, parameter, value.c_str(), value.length(), SQLITE_TRANSIENT) != SQLITE_OK)
      throw Db::error(sqlite3_errmsg(db_.db_), sqlite3_sql(stmt_));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Binds a byte array value to the specified parameter.
///
/// \param  parameter The index of the parameter to bind.
/// \param  value The byte array value to bind to the parameter.
/// \param  length The number of bytes in the byte array.
void Stmt::bindBlob(int parameter, const void* value, int length)
{
   if (sqlite3_bind_blob(stmt_, parameter, value, length, SQLITE_TRANSIENT) != SQLITE_OK)
      throw Db::error(sqlite3_errmsg(db_.db_), sqlite3_sql(stmt_));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Binds a static byte array value to the specified parameter.
///
/// \details The parameter must be located in static memory, since it may be
///         accessed at a later point.  For instance, this should be used over
///         bindBlob(int, const void*, int) when the value is a string literal.
///
/// \param  parameter The index of the parameter to bind.
/// \param  value The byte array value to bind to the parameter.
/// \param  length The number of bytes in the byte array.
void Stmt::bindBlob_s(int parameter, const void* value, int length)
{
   if (sqlite3_bind_blob(stmt_, parameter, value, length, SQLITE_STATIC) != SQLITE_OK)
      throw Db::error(sqlite3_errmsg(db_.db_), sqlite3_sql(stmt_));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Executes the statement.
///
/// \return \c true if a result set was returned and there is a row available.
bool Stmt::step()
{
   int result = sqlite3_step(stmt_);
   col_names_.reset();

   if (result == SQLITE_DONE)
      return false;
   if (result == SQLITE_ROW)
      return true;

   // otherwise an error occurred
   if (result == SQLITE_BUSY)
      throw Db::error("Database is busy.", sqlite3_sql(stmt_));
   if (result == SQLITE_MISUSE)
      throw Db::error("Statement shouldn't be executed now!", sqlite3_sql(stmt_));
   throw Db::error(sqlite3_errmsg(db_.db_), sqlite3_sql(stmt_));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Resets the statement after it has been executed so that it can be
///         executed again.
void Stmt::reset()
{
   sqlite3_reset(stmt_);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the number of columns in the result set.
///
/// \return The number of columns in the result set.
int Stmt::columns()
{
   return sqlite3_column_count(stmt_);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the column index of the column with the specified name.
/// 
/// \note   This is __much__ slower than specifying column indices directly.
///
/// \param  name The name of the column we are interested in.
/// \return The index of the column with the specified name.
int Stmt::column(const std::string& name)
{
   if (!col_names_)
   {
      col_names_.reset(new std::unordered_map<std::string, int>());
      int cols = columns();
      for (int i = 0; i < cols; ++i)
         col_names_->insert(std::pair<std::string,int>(sqlite3_column_name(stmt_, i), i));
   }

   std::unordered_map<std::string, int>::iterator it = col_names_->find(name);
   if (it != col_names_->end())
      return it->second;
   throw Db::error(std::string("Column '").append(name).append("' not found!"), sqlite3_sql(stmt_));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the SQLite type code of the column specified.
///
/// \details See [SQLite C Interface Fundamental
///          Datatypes](http://www.sqlite.org/c3ref/c_blob.html) for details.
///
/// \param  column The column to retrieve the type of.
/// \return The type code of the requested column.
int Stmt::getType(int column)
{
   assert(column >= 0 && column < columns());
   return sqlite3_column_type(stmt_, column);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the specified column as a bool.
///
/// \details Any value that would result in getInt(column) returning a non-zero
///          value is considered \c true, while any other value is considered
///          \c false.
///
/// \param  column The column to retrieve.
/// \return The value of the column.
bool Stmt::getBool(int column)
{
   assert(column >= 0 && column < columns());
   return sqlite3_column_int(stmt_, column) != 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the specified column as an integer value.
///
/// \param  column The column to retrieve.
/// \return The value of the column.
int Stmt::getInt(int column)
{
   assert(column >= 0 && column < columns());
   return sqlite3_column_int(stmt_, column);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the specified column as an unsigned integer value.
///
/// \details If the value is negative, it will be represented in 2's complement
///         format.
/// 
/// \param  column The column to retrieve.
/// \return The value of the column.
unsigned int Stmt::getUInt(int column)
{
   assert(column >= 0 && column < columns());
   return static_cast<unsigned int>(sqlite3_column_int64(stmt_, column));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the specified column as a 64-bit integer value.
///
/// \param  column The column to retrieve.
/// \return The value of the column.
sqlite3_int64 Stmt::getInt64(int column)
{
   assert(column >= 0 && column < columns());
   return sqlite3_column_int64(stmt_, column);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the specified column as an unsigned 64-bit integer value.
///
/// \details If the value is negative, it will be represented in 2's complement
///         format.
///
/// \param  column The column to retrieve.
/// \return The value of the column.
sqlite3_uint64 Stmt::getUInt64(int column)
{
   assert(column >= 0 && column < columns());
   return static_cast<sqlite3_uint64>(sqlite3_column_int64(stmt_, column));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the specified column as a 64-bit floating point value.
///
/// \param  column The column to retrieve.
/// \return The value of the column.
double Stmt::getDouble(int column)
{
   assert(column >= 0 && column < columns());
   return sqlite3_column_double(stmt_, column);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the specified column as a text string value.
///
/// \param  column The column to retrieve.
/// \return The value of the column.
const char* Stmt::getText(int column)
{
   assert(column >= 0 && column < columns());
   return reinterpret_cast<const char*>(sqlite3_column_text(stmt_, column));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the specified column as a binary string value.
///
/// \param  column The column to retrieve.
/// \return The value of the column.
std::string Stmt::getBlob(int column)
{
   assert(column >= 0 && column < columns());
   int bytes = sqlite3_column_bytes(stmt_, column);
   return std::string(reinterpret_cast<const char*>(sqlite3_column_blob(stmt_, column)), bytes);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the specified column as a byte array.
///
/// \param  column The column to retrieve.
/// \param  dest A pointer to store the location of the byte array.
/// \return The number of bytes in the array.
int Stmt::getBlob(int column, const void*& dest)
{
   assert(column >= 0 && column < columns());
   int bytes = sqlite3_column_bytes(stmt_, column);
   dest = sqlite3_column_blob(stmt_, column);
   return bytes;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Prints the uncompiled SQL source a statement object.
///
/// \param  os The \c std::ostream to output to.
/// \param  s The statement to output.
/// \return The \c std::ostream object provided, to allow operator chaining.
///
/// \relates Stmt
std::ostream& operator<<(std::ostream& os, const Stmt& s)
{
   return os << s.getSql();
}

} // namespace be::bed
} // namespace be
