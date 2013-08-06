///////////////////////////////////////////////////////////////////////////////
/// \file   be/bed/cached_stmt.h
/// \author Benjamin Crist
///
/// \brief  be::bed::CachedStmt class header.

#ifndef BE_BED_CACHED_STMT_H_
#define BE_BED_CACHED_STMT_H_

#include "be/bed/stmt.h"

#include <ostream>

namespace be {
namespace bed {

class StmtCache;

///////////////////////////////////////////////////////////////////////////////
/// \class  CachedStmt   be/bed/cached_stmt.h "be/bed/cached_stmt.h"
///
/// \brief  Used exactly like bed::Stmt, except that CachedStmt objects cannot
///         be directly constructed.
/// \details Instead, they are returned from StmtCache::hold() calls and
///         represent statement objects owned by the cache.  When a CachedStmt
///         is destroyed, it automatically releases its statement object back
///         to the cache that created it so that it may be used elsewhere.
///
///         CachedStmts may be move-constructed.  After the move, the new
///         object owns the 'hold' on the statement, and the old object will
///         not release it back to the cache when it is destroyed.  While
///         move-construction is allowed, there is no move-assignment or
///         default constructor, so CachedStmts cannot be placed in standard
///         library containers (there's no reason to do such a thing to begin
///         with).
/// \ingroup db
class CachedStmt
{
   friend class StmtCache;
public:
   CachedStmt(CachedStmt&& other);
   ~CachedStmt();

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
   CachedStmt(StmtCache* cache, Stmt& stmt);

   StmtCache* cache_;
   Stmt& stmt_;

   CachedStmt(const CachedStmt&);
   void operator=(const CachedStmt&);
};

std::ostream& operator<<(std::ostream& os, const CachedStmt& s);

} // namespace be::bed
} // namespace be

#endif
