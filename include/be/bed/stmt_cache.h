///////////////////////////////////////////////////////////////////////////////
/// \file   be/bed/stmt_cache.h
/// \author Benjamin Crist
///
/// \brief  be::bed::StmtCache class header.

#ifndef BE_BED_STMT_CACHE_H_
#define BE_BED_STMT_CACHE_H_

#include "be/bed/detail/stmt_cache_entry.h"
#include "be/bed/cached_stmt.h"
#include "be/id.h"

#include <mutex>
#include <unordered_map>

#define BE_BED_STMT_CACHE_DEFAULT_MAX_SIZE 24

namespace be {
namespace bed {

class Db;

///////////////////////////////////////////////////////////////////////////////
/// \class  StmtCache   be/bed/stmt_cache.h "be/bed/stmt_cache.h"
///
/// \brief Caches Stmt objects using an LRU (least recently used) algorithm.
/// \details The cache stores a number of compiled SQL statements (Stmt
///        objects) which can be checked out (held) by clients of the database.
///        The cache has a specific capacity.  When the size of the cache
///        increases beyond capacity, the cache will attempt to destroy the
///        least recently held statements until the cache's capacity is
///        reached.  Note that if there are no unheld statements the cache's
///        capacity may increase beyond its capacity.
///
///        When a database client wants a particular statement, they call one
///        of the hold() functions.  When the client is finished using the
///        statement that was returned, they let it go out of scope, which
///        causes it to inform the cache the underlying Stmt object can be
///        used again.
///
///        The cache is synchronized for concurrent access from multiple
///        threads.
///
///        Users of StmtCache must ensure that the lifetime of the StmtCache
///        (and the Stmt objects owned by it) is shorter than the lifetime of
///        the database with which it is associated.  This isn't usually a
///        problem since usually a Bed object's StmtCache is used, and the
///        bed owns both the cache and Db object and ensures that the cache
///        is cleaned up before the Db.
/// \ingroup db
class StmtCache
{
   friend class CachedStmt;
public:
   StmtCache(Db& db);
   StmtCache(Db& db, size_t capacity);
   ~StmtCache();

   void setCapacity(size_t capacity);
   size_t getCapacity();

   size_t getSize();
   size_t getHeldSize();

   CachedStmt hold(const std::string& sql);
   CachedStmt hold(const char* sql);
   CachedStmt hold(const Id& id, const std::string& sql);
   CachedStmt hold(const Id& id, const char* sql);

private:
   void hold_(detail::StmtCacheEntry& entry);
   void release_(Stmt& stmt);
   void checkSize_();

   std::mutex mutex_;

   Db& db_;

   size_t capacity_;
   size_t held_size_;

   size_t next_index_;
   size_t oldest_index_;

   std::unordered_multimap<Id, detail::StmtCacheEntry> cache_;
};

} // namespace be::bed
} // namespace be

#endif
