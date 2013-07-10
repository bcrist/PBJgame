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
