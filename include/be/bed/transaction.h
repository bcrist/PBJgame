///////////////////////////////////////////////////////////////////////////////
/// \file   be/bed/transaction.h
/// \author Benjamin Crist
///
/// \brief  be::bed::Transaction class header.

#ifndef BE_BED_TRANSACTION_H_
#define BE_BED_TRANSACTION_H_
#include "be/_be.h"

namespace be {
namespace bed {

class Db;

///////////////////////////////////////////////////////////////////////////////
/// \class  Transaction   be/bed/transaction.h "be/bed/transaction.h"
///
/// \brief  RAII transaction wrapper for be::bed::Db and be::bed::Stmt.
/// \details When a Transaction object is constructed, it begins a transaction
///         on the database.  When it is destroyed (goes out of scope) it will
///         automatically roll back the transaction if it has not been comitted
///         yet.
/// \ingroup db
class Transaction
{   
public:
   enum Type
   {
      Deferred,
      Immediate,
      Exclusive
   };

   explicit Transaction(Db& db);   
   Transaction(Db& db, Type type);
   ~Transaction();

   void save(const std::string& savepoint);
   void release(const std::string& savepoint);
   void rollback(const std::string& savepoint);

   void rollback();
   void commit();

private:
   Db& db_;
   bool active_;

   Transaction(const Transaction&);
   void operator=(const Transaction&);
};

} // namespace be::bed
} // namespace be

#endif
