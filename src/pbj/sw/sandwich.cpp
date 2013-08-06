///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/sw/sandwich.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::sw::Sandwich functions.

#include "pbj/sw/sandwich.h"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to get the Id of a DbFile.
/// \details The Id is stored in the 'id' property of the
///         'pbj_sandwich_properties' table.
#define PBJ_SW_SANDWICH_SQL_GET_ID "SELECT value FROM sw_sandwich_properties WHERE property = 'id' LIMIT 1"

#ifdef BE_ID_NAMES_ENABLED
#define PBJ_SW_SANDWICH_SQLID_GET_ID     PBJ_SW_SANDWICH_SQL_GET_ID
#else
// TODO: precalculate SQLID using idgen.exe
#define PBJ_SW_SANDWICH_SQLID_GET_ID     PBJ_SW_SANDWICH_SQL_GET_ID
#endif
namespace pbj {
namespace sw {

Sandwich::Sandwich(const std::string& path, bool read_only)
   : db_(path, read_only ? SQLITE_OPEN_READONLY : SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE),
     stmt_cache_(db_)
{
    db::CachedStmt& get_id(stmt_cache_.hold(Id(PBJ_SW_SANDWICH_SQLID_GET_ID), PBJ_SW_SANDWICH_SQL_GET_ID));
    if (get_id.step())
        id_ = Id(get_id.getUInt64(0));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves this bed's ID (usually hashed from the database path).
///
/// \return The bed's Id.
const Id& Sandwich::getId() const
{
   return id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the Db object representing the connection to this bed's
///         database file.
///
/// \return The Db object associated with the bed.
db::Db& Sandwich::getDb()
{
   return db_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a StmtCache object which can be used for executing
///         statements and queries on this bed's database.
///
/// \return The bed's StmtCache.
db::StmtCache& Sandwich::getStmtCache()
{
   return stmt_cache_;
}

} // namespace pbj::sw
} // namespace pbj
