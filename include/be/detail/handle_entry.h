///////////////////////////////////////////////////////////////////////////////
/// \file   be/detail/handle_entry.h
/// \author Benjamin Crist
///
/// \brief  be::detail::HandleEntry declaration & implementation.

#ifndef BE_DETAIL_HANDLE_ENTRY_H_
#define BE_DETAIL_HANDLE_ENTRY_H_

#include "be/_be.h"

#include <cstdint>

namespace be {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
/// \struct HandleEntry   be/detail/handle_entry.h " be/detail/handle_entry.h"
///
/// \brief  Describes an object in a HandleManager which can be referenced by
///         handles.
/// \tparam T The type of object that is being handled.
template <class T>
struct HandleEntry
{
   ////////////////////////////////////////////////////////////////////////////
   /// \brief  Constructs a HandleEntry using the provided check value and
   ///         target pointer.
   HandleEntry(uint32_t check, T* target)
      : check(check), target(target) {}

   /// \brief  Value used to determine if a handle is valid.
   /// \details When the MSB of check is set, it means this entry refers to a
   ///         valid object somewhere.  When the MSB is not set, it means
   ///         the entry is currently invalid; it can be reused for another
   ///         handle.
   ///
   ///         Each time an entry is used for a new handle, the lower 31 bits
   ///         are incremented once.  A handle's check value must match the
   ///         check value in the HandleEntry it's index refers to,
   ///         otherwise the handle doesn't refer to the same object.
   uint32_t check;      
   union {
      T* target;        ///< If a valid entry, points to the handled object.
      size_t next_free; ///< If this is an unused entry, this is the index of the next unused HandleEntry index.
   };
};

} // namespace be::detail
} // namespace be

#endif
