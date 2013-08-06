///////////////////////////////////////////////////////////////////////////////
/// \file   be/bed/detail/stmt_cache_entry.h
/// \author Benjamin Crist
///
/// \brief  be::bed::detail::StmtCacheEntry class header.

#ifndef BE_BED_DETAIL_STMT_CACHE_ENTRY_H_
#define BE_BED_DETAIL_STMT_CACHE_ENTRY_H_

#include "be/bed/stmt.h"

#include <memory>

namespace be {
namespace bed {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
/// \class  StmtCacheEntry   be/bed/detail/stmt_cache_entry.h "be/bed/detail/stmt_cache_entry.h"
///
/// \brief  Used by StmtCache to keep track of which statements are currently
///         held or unheld, and how recently each statement was held.
struct StmtCacheEntry
{
public:
   StmtCacheEntry();
   StmtCacheEntry(Stmt* stmt);

   StmtCacheEntry(StmtCacheEntry&& other);
   void operator=(StmtCacheEntry&& other);

   bool held;
   size_t access_index;
   std::unique_ptr<Stmt> stmt;

private:
   StmtCacheEntry(const StmtCacheEntry&);
   void operator=(const StmtCacheEntry&);
};

} // namespace be::bed::detail
} // namespace be::bed
} // namespace be

#endif
