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
