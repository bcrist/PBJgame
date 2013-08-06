///////////////////////////////////////////////////////////////////////////////
/// \file   be/detail/handle_manager.inl
/// \author Benjamin Crist
///
/// \brief  Implementations of be::detail::HandleManager template functions.

#if !defined(BE_DETAIL_HANDLE_MANAGER_H_) && !defined(DOXYGEN)
#include "be/detail/handle_manager.h"
#elif !defined(BE_DETAIL_HANDLE_MANAGER_INL_)
#define BE_DETAIL_HANDLE_MANAGER_INL_

namespace be {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a HandleManager.
template <class T>
HandleManager<T>::HandleManager()
   : first_free(0),
     last_free(0)
{
   entries.reserve(64);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Adds a new entry to this handle manager and sets the handle
///         variables provided to refer to the new handle.
/// \param  index A reference to the \ref be::SourceHandle::index_ "index"
///         member of the SourceHandle associated with the object.
/// \param  check A reference to the \ref be::SourceHandle::check_ "check"
///         member of the SourceHandle associated with the object.
/// \param  target The address of the object that the SourceHandle is
///         associated with.
template <class T>
void HandleManager<T>::add(uint32_t& index, uint32_t& check, T* target)
{
   if (first_free >= entries.size())
   {
      index = entries.size();
      entries.push_back(HandleEntry<T>(0x80000000, target));
      first_free = entries.size();
      last_free = entries.size();
   }
   else
   {
      // first_free is inside vector
      index = first_free;
      first_free = entries[first_free].next_free;
      if (last_free == index)
         last_free = first_free;
      entries[index].target = target;
      entries[index].check |= 0x80000000;
      entries[index].check += 1;
   }

   check = entries[index].check;
   ++size;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Updates an existing handle to point to a new target.  Existing
///         handles will remain valid, but now point to the new target.
/// \param  index Determines the HandleEntry to update.
/// \param  target The new address of the handled object.
template <class T>
void HandleManager<T>::update(uint32_t index, T* target)
{
   entries[index].target = target;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Invalidates an existing handle, freeing that index for use by new
///         handles and invalidating old handles.
/// \details After an index has been used 2^31 (approximately 2 billion) times,
///         there are no more usable check values, so the index will not be
///         added to the free list and will not be used again until the process
///         ends.
/// \param  index Determines the HandleEntry to invalidate.
template <class T>
void HandleManager<T>::invalidate(uint32_t index)
{
   entries[index].check &= 0x7FFFFFFF;
   if (entries[index].check != 0x7FFFFFFF)
   {
      // only add this index to the free list if there is at least 1 usable check value left.

      entries[index].next_free = entries.size();

      if (last_free >= entries.size())
         first_free = index;
      else
         entries[last_free].next_free = index;
      last_free = index;
   }
   --size;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a handle manager for a specific type.
template <class T>
inline HandleManager<T>& getHandleManager()
{
   static HandleManager<T> manager;
   return manager;
}

} // namespace be::detail
} // namespace be

#endif
