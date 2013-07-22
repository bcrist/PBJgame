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
/// \file   be/bed/bed.h
/// \author Benjamin Crist
///
/// \brief  be::bed::Bed class header.

#ifndef BE_BED_BED_H_
#define BE_BED_BED_H_

#include "be/bed/db.h"
#include "be/bed/stmt.h"
#include "be/bed/stmt_cache.h"
#include "be/id.h"
#include "be/source_handle.h"

#include <memory>

#define BE_BED_SCHEMA_VERSION_MAJOR 0
#define BE_BED_SCHEMA_VERSION_MINOR 2

namespace be {
namespace bed {

///////////////////////////////////////////////////////////////////////////////
/// \class  Bed   be/bed/bed.h "be/bed/bed.h"
///
/// \brief  Represents a bengine database file which can be used to access
///         persistent game data.
/// \ingroup db
class Bed : public std::enable_shared_from_this<Bed>
{
public:
   Bed(const Id& id, const std::string& path, bool read_only);

   const Id& getId() const;

   Db& getDb();
   StmtCache& getStmtCache();

private:
   Id id_;
   Db db_;
   StmtCache stmt_cache_;
   

   unsigned int schema_version_;

   Bed(const Bed&);
   void operator=(const Bed&);
};

} // namespace be::bed
} // namespace be

#endif
