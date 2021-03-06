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
/// \file   be/bed/stmt_cache.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of be::bed::StmtCache functions.

#include "be/bed/stmt_cache.h"

#include "be/bed/stmt.h"

#include <iostream>

namespace be {
namespace bed {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a Stmt cache for the provided database using the default
///         maximum cache size.
///
/// \param  db The database on which the cache's statements will operate.
StmtCache::StmtCache(Db& db)
   : db_(db),
     capacity_(BE_BED_STMT_CACHE_DEFAULT_MAX_SIZE),
     held_size_(0),
     next_index_(1),
     oldest_index_(1)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a Stmt cache for the provided database using the
///         specified maximum cache size.
///
/// \param  db The database on which the cache's statements will operate.
/// \param  capacity The maximum number of statements to cache.
StmtCache::StmtCache(Db& db, size_t capacity)
   : db_(db),
     capacity_(capacity),
     held_size_(0),
     next_index_(1),
     oldest_index_(1)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destroys this asset cache, along with all Stmt objects owned by it.
///
/// \details If any statements have not been released by the time the
///         destructor runs, a warning will be emitted.  If DEBUG is defined
///         the warning will include a list of the IDs of the statements that
///         have not been released.
StmtCache::~StmtCache()
{
   if (held_size_ > 0)
   {
      BE_LOG(VWarning) << "StmtCache being destroyed with held statement(s)!" << BE_LOG_NL
                       << "     Held Size: " << held_size_;

#ifdef DEBUG
      int index = 1;
      for (auto i(cache_.begin()), end(cache_.end()); i != end; ++i)
      {
         detail::StmtCacheEntry& entry = i->second;

         if (entry.held)
            BE_LOG_STREAM << BE_LOG_NL << "Held Stmt " << index++ << " ID: " << entry.stmt->getId();
      }
#endif

      BE_LOG_STREAM << BE_LOG_END;
   }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the capacity of this cache.
///
/// \param  capacity The new maximum number of statements to cache.
void StmtCache::setCapacity(size_t capacity)
{
   // lock mutex
   std::lock_guard<std::mutex> lock(mutex_);

   capacity_ = capacity;
   checkSize_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the current capacity of this cache.
///
/// \return The current capacity of the cache.
size_t StmtCache::getCapacity()
{
   // lock mutex
   std::lock_guard<std::mutex> lock(mutex_);

   return capacity_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the number of statements currently in the cache.
///
/// \details Includes both held and unheld statements.
///
/// \note   May be larger than getCapacity().
///
/// \return The current number of statements in the cache.
size_t StmtCache::getSize()
{
   // lock mutex
   std::lock_guard<std::mutex> lock(mutex_);

   return cache_.size();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the number of held statements in the cache.
///
/// \return The number of statements that are currently in use.
size_t StmtCache::getHeldSize()
{
   // lock mutex
   std::lock_guard<std::mutex> lock(mutex_);

   return held_size_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Holds a statement using the provided SQL and an Id generated by
///         hashing the SQL text.
///
/// \details Statements with the same Id are assumed to have the same 
///         functionality, so if there is an unheld statement with the same Id
///         already in the cache, it is marked as held and returned.  Otherwise
///         a new statement is constructed using this cache's database and SQL
///         provided.  The new statement is then added to the cache as a held
///         statement.
///
/// \param  sql The SQL text of the statement to compile.
/// \return A CachedStmt representing the held statement.
CachedStmt StmtCache::hold(const std::string& sql)
{
   return hold(Id(sql), sql.c_str());
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Holds a statement using the provided SQL and an Id generated by
///         hashing the SQL text.
///
/// \details Statements with the same Id are assumed to have the same 
///         functionality, so if there is an unheld statement with the same Id
///         already in the cache, it is marked as held and returned.  Otherwise
///         a new statement is constructed using this cache's database and SQL
///         provided.  The new statement is then added to the cache as a held
///         statement.
///
/// \param  sql The SQL text of the statement to compile.
/// \return A CachedStmt representing the held statement.
CachedStmt StmtCache::hold(const char* sql)
{
   return hold(Id(sql), sql);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Holds a statement using the provided SQL and an Id.
///
/// \details Statements with the same Id are assumed to have the same 
///         functionality, so if there is an unheld statement with the same Id
///         already in the cache, it is marked as held and returned.  Otherwise
///         a new statement is constructed using this cache's database and SQL
///         provided.  The new statement is then added to the cache as a held
///         statement.
///
/// \param  id The Id of the statement.
/// \param  sql The SQL text of the statement to compile.
/// \return A CachedStmt representing the held statement.
CachedStmt StmtCache::hold(const Id& id, const std::string& sql)
{
   return hold(id, sql.c_str());
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Holds a statement using the provided SQL and Id.
/// 
/// \details Statements with the same Id are assumed to have the same 
///         functionality, so if there is an unheld statement with the same Id
///         already in the cache, it is marked as held and returned.  Otherwise
///         a new statement is constructed using this cache's database and SQL
///         provided.  The new statement is then added to the cache as a held
///         statement.
///
/// \param  id The Id of the statement.
/// \param  sql The SQL text of the statement to compile.
/// \return A CachedStmt representing the held statement.
CachedStmt StmtCache::hold(const Id& id, const char* sql)
{
   // First check to see if there's an existing unheld statement we can use
   std::unique_lock<std::mutex> lock(mutex_);
   if (held_size_ < cache_.size())
   {
      // it only makes sense to check for a specific unheld statement
      // if there is at least one unheld statement in the cache.

      auto range(cache_.equal_range(id));

      for (auto i(range.first); i != range.second; ++i)
      {
         detail::StmtCacheEntry& entry = i->second;

         if (!entry.held)
         {
            hold_(entry);
            return CachedStmt(this, *entry.stmt);
         }
      }
   }
   lock.unlock(); // unlock mutex while SQL compiles

   // If not, release the mutex lock and construct a new statement
   // SQL statement compilation can be an expensive operation, so
   // we don't want to prevent other threads from accessing the cache
   // while SQLite is doing its thing.
   Stmt* stmt = new Stmt(db_, id, sql);

   // re-lock mutex and insert the new statement object
   lock.lock();
   hold_(cache_.insert(std::make_pair(id, detail::StmtCacheEntry(stmt)))->second);
   checkSize_();

   return CachedStmt(this, *stmt);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Releases a statement previously returned by a call to hold().
///
/// \details The statement will automatically have reset() and bind() called to
///         restore its original state.  After being released, the statement
///         may be returned by future calls to hold().
///
/// \param  stmt The statement to release.
void StmtCache::release_(Stmt& stmt)
{
   const Id& id = stmt.getId();
   stmt.reset(); // ensure statement is in a clean state
   stmt.bind();  // release any bound parameters

   {
      std::lock_guard<std::mutex> lock(mutex_);
      auto range(cache_.equal_range(id));

      for (auto i(range.first); i != range.second; ++i)
      {
         detail::StmtCacheEntry& entry = i->second;

         if (entry.stmt.get() == &stmt && entry.held)
         {
            entry.held = false;
            --held_size_;
            checkSize_();
            return;
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Helper function to hold an existing entry in the cache.
///
/// \note   StmtCache::mutex_ must be locked before calling this function!
///
/// \param  entry A reference to an entry owned by StmtCache::cache_.
void StmtCache::hold_(detail::StmtCacheEntry& entry)
{
   entry.held = true;
   entry.access_index = next_index_++;
   ++held_size_;

   if (next_index_ == 0)
   {
      // when next_index_ overflows, set all held access indices to 0

      next_index_ = 1;
      oldest_index_ = 1;
      for (auto i(cache_.begin()), end(cache_.end()); i != end; ++i)
         i->second.access_index = 0;
   }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Helper function to reduce the size of the cache if it is too big.
///
/// \note   StmtCache::mutex_ must be locked before calling this function!
void StmtCache::checkSize_()
{
   while (cache_.size() > capacity_ && held_size_ < cache_.size())
   {
      // remove the oldest entry
      size_t oldest_index = 0;
      auto oldest = cache_.end();

      for (auto i(cache_.begin()), end(cache_.end()); i != end; ++i)
      {
         detail::StmtCacheEntry& entry = i->second;

         if (!entry.held)
         {
            if (entry.access_index == 0)
            {
               // if this entry is older than the last overflow of next_index_
               // then we can remove it.
               cache_.erase(i);
               oldest_index = 0; // don't call erase(oldest) at end of while()
               break;
            }

            if (entry.access_index == oldest_index_)
            {
               // if we know this entry is the oldest, theres no point in looking
               // at any further entries.
               oldest_index = oldest_index_;
               oldest = i; // we will remove this at end of while()
               break;
            }

            // if we get here, check to see if this entry is older than 'oldest'
            if (entry.access_index < oldest_index || oldest_index == 0)
            {
               oldest_index = entry.access_index;
               oldest = i; // we will remove this at end of while()
            }
         }
      }

      if (oldest_index > 0)
      {
         oldest_index_ = oldest_index + 1;
         cache_.erase(oldest);
      }
   } // remove oldest unheld entry until size < max size or there are no unheld entries.
}

} // namespace be::bed
} // namespace be
