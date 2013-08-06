///////////////////////////////////////////////////////////////////////////////
/// \file   be/bed/cached_stmt.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of be::bed::CachedStmt functions.

#include "be/bed/cached_stmt.h"

#include "be/bed/stmt.h"
#include "be/bed/stmt_cache.h"

namespace be {
namespace bed {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Move-constructs a CachedStmt.
///
/// \details Moves the hold placed on a statement to a new object.  When the
///         new object is destroyed, the statement will be released back to the
///         cache it came from (but not when the first object is destroyed).
///         The source of the move may still be safely used to access the
///         cached statement as long as the moved object exists, but this is
///         not recommended.
///
/// \param  other The CachedStmt to move.
CachedStmt::CachedStmt(CachedStmt&& other)
   : cache_(other.cache_),
     stmt_(other.stmt_)
{
   other.cache_ = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destructor.
///
/// \details Releases this statement back to the cache it came from.
CachedStmt::~CachedStmt()
{
   if (cache_)
      cache_->release_(stmt_);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::getDb()
Db& CachedStmt::getDb()
{
   return stmt_.getDb();
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::getId() const
const Id& CachedStmt::getId() const
{
   return stmt_.getId();
}


///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::getSql() const
const char* CachedStmt::getSql() const
{
   return stmt_.getSql();
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::parameters()
int CachedStmt::parameters()
{
   return stmt_.parameters();
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::parameter(const std::string&)
int CachedStmt::parameter(const std::string& name)
{
   return stmt_.parameter(name);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::bind()
void CachedStmt::bind()
{
   stmt_.bind();
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::bind(int)
void CachedStmt::bind(int parameter)
{
   stmt_.bind(parameter);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::bind(int, bool)
void CachedStmt::bind(int parameter, bool value)
{
   stmt_.bind(parameter, value);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::bind(int, int)
void CachedStmt::bind(int parameter, int value)
{
   stmt_.bind(parameter, value);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::bind(int, unsigned int)
void CachedStmt::bind(int parameter, unsigned int value)
{
   stmt_.bind(parameter, value);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::bind(int, sqlite3_int64)
void CachedStmt::bind(int parameter, sqlite3_int64 value)
{
   stmt_.bind(parameter, value);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::bind(int, sqlite3_uint64)
void CachedStmt::bind(int parameter, sqlite3_uint64 value)
{
   stmt_.bind(parameter, value);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::bind(int, double)
void CachedStmt::bind(int parameter, double value)
{
   stmt_.bind(parameter, value);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::bind(int, const std::string&)
void CachedStmt::bind(int parameter, const std::string& value)
{
   stmt_.bind(parameter, value);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::bind(int, const char*)
void CachedStmt::bind(int parameter, const char* value)
{
   stmt_.bind(parameter, value);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::bind_s(int, const char*)
void CachedStmt::bind_s(int parameter, const char* value)
{
   stmt_.bind_s(parameter, value);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::bindBlob(int, const std::string&)
void CachedStmt::bindBlob(int parameter, const std::string& value)
{
   stmt_.bindBlob(parameter, value);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::bindBlob(int, const void*, int)
void CachedStmt::bindBlob(int parameter, const void* value, int length)
{
   stmt_.bindBlob(parameter, value, length);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::bindBlob_s(int, const void*, int)
void CachedStmt::bindBlob_s(int parameter, const void* value, int length)
{
   stmt_.bindBlob_s(parameter, value, length);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::step()
bool CachedStmt::step()
{
   return stmt_.step();
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::reset()
void CachedStmt::reset()
{
   stmt_.reset();
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::columns()
int CachedStmt::columns()
{
   return stmt_.columns();
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::column(const std::string&)
int CachedStmt::column(const std::string& name)
{
   return stmt_.column(name);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::getType(int)
int CachedStmt::getType(int column)
{
   return stmt_.getType(column);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::getBool(int)
bool CachedStmt::getBool(int column)
{
   return stmt_.getBool(column);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::getInt(int)
int CachedStmt::getInt(int column)
{
   return stmt_.getInt(column);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::getUInt(int)
unsigned int CachedStmt::getUInt(int column)
{
   return stmt_.getUInt(column);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::getInt64(int)
sqlite3_int64 CachedStmt::getInt64(int column)
{
   return stmt_.getInt64(column);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::getUInt64(int)
sqlite3_uint64 CachedStmt::getUInt64(int column)
{
   return stmt_.getUInt64(column);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::getDouble(int)
double CachedStmt::getDouble(int column)
{
   return stmt_.getDouble(column);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::getText(int)
const char* CachedStmt::getText(int column)
{
   return stmt_.getText(column);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::getBlob(int)
std::string CachedStmt::getBlob(int column)
{
   return stmt_.getBlob(column);
}

///////////////////////////////////////////////////////////////////////////////
/// \copydoc   Stmt::getBlob(int, const void*&)
int CachedStmt::getBlob(int column, const void*& dest)
{
   return stmt_.getBlob(column, dest);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a CachedStmt.
///
/// \details Called from StmtCache to create cached stmts.
///
/// \param  cache The StmtCache that manages this cached statement.
/// \param  stmt The Stmt object which should be used for all accesses.
CachedStmt::CachedStmt(StmtCache* cache, Stmt& stmt)
   : cache_(cache),
     stmt_(stmt)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Prints the uncompiled SQL source a statement object.
///
/// \param  os The \c std::ostream to output to.
/// \param  s The statement to output.
/// \return The \c std::ostream object provided, to allow operator chaining.
///
/// \relates CachedStmt
std::ostream& operator<<(std::ostream& os, const CachedStmt& s)
{
   return os << s.getSql();
}

} // namespace be::bed
} // namespace be
