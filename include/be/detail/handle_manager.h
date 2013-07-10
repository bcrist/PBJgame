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
/// \file   be/detail/handle_manager.h
/// \author Benjamin Crist
///
/// \brief  be::detail::HandleManager class header.

#ifndef BE_DETAIL_HANDLE_MANAGER_H_
#define BE_DETAIL_HANDLE_MANAGER_H_

#include "be/detail/handle_entry.h"

#include <vector>

namespace be {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
/// \class  HandleManager   be/detail/handle_manager.h "be/detail/handle_manager.h"
///
/// \brief  A HandleManager is a static object which is used to convert a
///         handle into a pointer to the object it points to.
/// \details A single HandleManager exists for each type of object that can
///         have handles, i.e. each specialization of SourceHandle, Handle
///         or ConstHandle.
/// \tparam T the type of object this manager deals with handles to.
template <class T>
struct HandleManager
{
   HandleManager();

   void add(uint32_t& index, uint32_t& check, T* target);
   void update(uint32_t index, T* target);
   void invalidate(uint32_t index);

   std::vector<HandleEntry<T>> entries;   ///< Holds the address of all currently valid objects which can have handles to them.
   size_t first_free;                     ///< Index in entries of the first unused (invalid) entry.  When an entry is added, it is located at this index.  Then first_free becomes the old entry's next_free index.
   size_t last_free;                      ///< Index in entries of the last unused (invalid) entry.  When an entry is invalidated, this element's next_free is set to the newly invalidated entry and last_free is set to the new entry's index.
   size_t size;                           ///< Holds the number of valid entries present in entries.  Not necessarily equal to entries.size().
};

template <class T>
HandleManager<T>& getHandleManager();

} // namespace be::detail
} // namespace be

#include "be/detail/handle_manager.inl"

#endif
