///////////////////////////////////////////////////////////////////////////////
/// \file   be/bed/detail/stmt_cache_entry.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of be::bed::detail::StmtCacheEntry functions.

#include "be/bed/detail/stmt_cache_entry.h"

namespace be {
namespace bed {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Default constructor.
///
/// \details Constructs an entry in an invalid state.  stmt must still be
///         initialized before the object is usable.
StmtCacheEntry::StmtCacheEntry()
   : held(false),
     access_index(0)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new entry which owns the statement object provided.
///
/// \details The Stmt provided must have been created using new Stmt(...).
///
/// \param  stmt The statement object used for this entry.
StmtCacheEntry::StmtCacheEntry(Stmt* stmt)
   : held(false),
     access_index(0),
     stmt(stmt)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Move-constructs an existing entry.
///
/// \param  other An existing entry to move from.
StmtCacheEntry::StmtCacheEntry(StmtCacheEntry&& other)
   : held(std::move(other.held)),
     access_index(std::move(other.access_index)),
     stmt(std::move(other.stmt))
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Move-assigns an existing entry.
///
/// \param  other An existing entry to move from.
void StmtCacheEntry::operator=(StmtCacheEntry&& other)
{
   held = std::move(other.held);
   access_index = std::move(other.access_index);
   stmt = std::move(other.stmt);
}

} // namespace be::bed::detail
} // namespace be::bed
} // namespace be
