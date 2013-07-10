// Copyright (c) 2013 Benjamin Crist
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

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
