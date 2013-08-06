///////////////////////////////////////////////////////////////////////////////
/// \file   be/bed/transaction.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of be::bed::Transaction functions.

#include "be/bed/transaction.h"

#include "be/bed/db.h"

#include <iostream>

namespace be {
namespace bed {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new transaction object and begins a transaction on the
///         provided database.
///
/// \details Transactions are not named, so no more than one Transaction
///         object should exist per Db object at any given time.
///
/// \param  db The database connection associated with this transaction.
Transaction::Transaction(Db& db)
   : db_(db),
     active_(true)
{
   db_.exec("BEGIN");
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new transaction object and begins a transaction on the
///         provided database.
///
/// \details Transactions are not named, so no more than one Transaction
///         object should exist per Db object at any given time.
///
///         The manner in which the database is locked depends on the
///         transaction type specified.  See [SQLite BEGIN
///         TRANSACTION](http://www.sqlite.org/lang_transaction.html) for more
///         information.
///
/// \param  db The database connection associated with this transaction.
/// \param  transaction_type The type of database locking to use with the
///         transaction.
Transaction::Transaction(Db& db, Type transaction_type)
   : db_(db),
     active_(true)
{
   const char* sql;
   switch (transaction_type)
   {
      case Immediate:
         sql = "BEGIN IMMEDIATE";
         break;

      case Exclusive:
         sql = "BEGIN EXCLUSIVE";
         break;

      case Deferred:
         sql = "BEGIN DEFERRED";
         break;

      default:
         sql = "BEGIN";
         break;
   }
   db_.exec(sql);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destroys the transaction object.
///
/// \details If the transaction is still pending (i.e. neither rollback() nor
///         commit() have been called yet), it will be automatically rolled
///         back.  If the rollback fails, no exception will be thrown, but a
///         warning message will be logged.
Transaction::~Transaction()
{
   try
   {
      rollback();
   }
   catch (const Db::error& err)
   {
      BE_LOG(VWarning) << "Failed to rollback transaction!" << BE_LOG_NL
                       << "Exception: " << err.what() << BE_LOG_NL
                       << "      SQL: " << err.sql() << BE_LOG_END;
   }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Adds a new savepoint transaction to the SQLite transaction stack.
///
/// \details This allows complex transactions to be only partially rolled back
///         if necessary.
///
/// \param  savepoint The name of the savepoint to create.
void Transaction::save(const std::string& savepoint)
{
   if (active_)
      db_.exec("SAVEPOINT " + savepoint);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Releases a previously added savepoint.
///
/// \details This makes it impossible to rollback to that savepoint in the
///         future, but no data is actually written out to the database until
///         the enclosing transaction is committed.  If the savepoint does not
///         exist, an exception will be thrown.
///
/// \param  savepoint The name of the previously created savepoint.
void Transaction::release(const std::string& savepoint)
{
   if (active_)
      db_.exec("RELEASE " + savepoint);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Rolls back the state of the database to the point where the
///         provided savepoint was created.
///
/// \details Any changes since that savepoint are discarded.  If the savepoint
///         does not exist, and exception will be thrown.
///
/// \param  savepoint The name of the previously created savepoint.
void Transaction::rollback(const std::string& savepoint)
{
   if (active_)
      db_.exec("ROLLBACK TO " + savepoint);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Rolls back the state of the database to the point where the
///         transaction object was created.
///
/// \details Any changes since that time are discarded, including any changes
///         made inside a savepoint transaction that has already been released.
void Transaction::rollback()
{
   if (active_)
   {
      db_.exec("ROLLBACK");
      active_ = false;
   }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Commits the transaction to the database.
///
/// \details If the commit fails, for example if a lock cannot be acquired on
///         the database, an exception is thrown.
void Transaction::commit()
{
   if (active_)
   {
      db_.exec("COMMIT");
      active_ = false;
   }
}

} // namespace be::bed
} // namespace be
