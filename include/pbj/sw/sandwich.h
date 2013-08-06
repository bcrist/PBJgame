///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/sw/sandwich.h
/// \author Benjamin Crist
///
/// \brief  pbj::sw::Sandwich class header.

#ifndef PBJ_SW_SANDWICH_H_
#define PBJ_SW_SANDWICH_H_

#include "be/bed/db.h"
#include "be/bed/stmt.h"
#include "be/bed/stmt_cache.h"
#include "be/id.h"
#include "be/source_handle.h"
#include "pbj/_pbj.h"

#include <memory>

namespace pbj {
namespace sw {

///////////////////////////////////////////////////////////////////////////////
/// \class  Sandwich   pbj/sw/sandwich.h "pbj/sw/sandwich.h"
///
/// \brief  Represents a database file which can be used to access persistent
///         game data.
class Sandwich : public std::enable_shared_from_this<Sandwich>
{
public:
   Sandwich(const std::string& path, bool read_only);

   const Id& getId() const;

   db::Db& getDb();
   db::StmtCache& getStmtCache();

private:
   Id id_;
   db::Db db_;
   db::StmtCache stmt_cache_;

   Sandwich(const Sandwich&);
   void operator=(const Sandwich&);
};

} // namespace be::bed
} // namespace be

#endif
