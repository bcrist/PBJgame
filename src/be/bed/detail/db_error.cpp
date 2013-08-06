///////////////////////////////////////////////////////////////////////////////
/// \file   be/bed/detail/db_error.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations for be::bed::detail::db_error functions.

#include "be/bed/detail/db_error.h"

namespace be {
namespace bed {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Construct a DB error with no SQL text provided.
///
/// \param  what_arg A description of the problem that caused the exception.
db_error::db_error(const std::string& what_arg)
   : std::runtime_error(what_arg)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Construct a DB error with no SQL text provided.
///
/// \param  what_arg A description of the problem that caused the exception.
db_error::db_error(const char* what_arg)
   : std::runtime_error(what_arg)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Construct a DB error with SQL text as context.
///
/// \param  what_arg A description of the problem that caused the exception.
/// \param  sql The SQL statement related to the error.
db_error::db_error(const std::string& what_arg, const std::string& sql)
   : std::runtime_error(what_arg),
     sql_(sql)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Construct a DB error with SQL text as context.
///
/// \param  what_arg A description of the problem that caused the exception.
/// \param  sql The SQL statement related to the error.
db_error::db_error(const char* what_arg, const char* sql)
   : std::runtime_error(what_arg),
     sql_(sql)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the SQL context text provided when the exception was
///         created.
///
/// \return The SQL statement provided in the constructor.
const std::string& db_error::sql() const
{
   return sql_;
}

} // namespace be::bed::detail
} // namespace be::bed
} // namespace be
