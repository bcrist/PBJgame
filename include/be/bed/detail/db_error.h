///////////////////////////////////////////////////////////////////////////////
/// \file   be/bed/detail/db_error.h
/// \author Benjamin Crist
///
/// \brief  be::bed::detail::db_error class header.

#ifndef BE_BED_DETAIL_DB_ERROR_H_
#define BE_BED_DETAIL_DB_ERROR_H_
#include "be/_be.h"

#include <exception>
#include <string>

namespace be {
namespace bed {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
/// \class  db_error be/bed/detail/db_error.h "be/bed/detail/db_error.h"
///
/// \brief Exception class for use by be::bed classes to represent problems
///        that occur while dealing with SQLite databases.
class db_error : public std::runtime_error
{
public:
   explicit db_error(const std::string& what_arg);
   explicit db_error(const char* what_arg);

   explicit db_error(const std::string& what_arg, const std::string& sql);
   explicit db_error(const char* what_arg, const char* sql);

   const std::string& sql() const;

private:
   std::string sql_;
};

} // namespace be::bed::detail
} // namespace be::bed
} // namespace be

#endif
