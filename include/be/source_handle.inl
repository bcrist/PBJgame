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
/// \file   be/source_handle.inl
/// \author Benjamin Crist
///
/// \brief  Implementations of be::SourceHandle template functions.

#if !defined(BE_SOURCE_HANDLE_H_) && !defined(DOXYGEN)
#include "be/source_handle.h"
#elif !defined(BE_SOURCE_HANDLE_INL_)
#define BE_SOURCE_HANDLE_INL_

namespace be {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a SourceHandle.
/// \details The handle can be copied and converted to other types of handle,
///         but the handle and all handles derived from it will remain invalid
///         until associate() is called to specify the target of the handle.
///         After that the handles will remain valid until the SourceHandle is
///         destroyed or moved.
template <class T>
inline SourceHandle<T>::SourceHandle()
   : Handle<T>()
{
   detail::HandleManager<T>& mgr(detail::getHandleManager<T>());
   mgr.add(index_, check_, nullptr);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Move-constructs an existing SourceHandle.
/// \details Handles which previously referred to the object associate()ed with
///         "other" will now refer to the object associate()ed with this
///         SourceHandle (after associate() is called).  From this point on,
///         "other" will be an invalidated handle.
/// \param  other The SourceHandle to move.
template <class T>
inline SourceHandle<T>::SourceHandle(SourceHandle<T>&& other)
   : Handle<T>(other)
{
   detail::HandleManager<T>& mgr(detail::getHandleManager<T>());
   mgr.update(index_, nullptr);

   other.check_ = 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Move-assigns another SourceHandle.
/// \details Handles which previously referred to the object associate()ed with
///         "other" will now refer to the object associate()ed with this
///         SourceHandle, and vice-versa.
/// \param  other The SourceHandle to swap with this one.
/// \return This SourceHandle.
template <class T>
inline SourceHandle<T>& SourceHandle<T>::operator=(SourceHandle<T>&& other)
{
   T* target = get();
   T* other_target = other.get();

   std::swap(index_, other.index_);
   std::swap(check_, other.check_);

   detail::HandleManager<T>& mgr(detail::getHandleManager<T>());

   if (check_)
      mgr.update(index_, target);

   if (other.check_)
      mgr.update(other.index_, other_target);

   return *this;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destroys a SourceHandle.
/// \details If the handle is valid, all handles referring to the object 
///         associate()ed with this SourceHandle will now be invalidated.
template <class T>
inline SourceHandle<T>::~SourceHandle()
{
   if (check_ == 0)
      return;

   detail::HandleManager<T>& mgr(detail::getHandleManager<T>());
   mgr.invalidate(index_);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Associates a target object with this handle.
/// \details This association needs to be done only once, and is not changed
///         when a SourceHandle is move-assigned.
/// \param  target The object to associate with this SourceHandle.
template <class T>
inline void SourceHandle<T>::associate(T* target)
{
   detail::HandleManager<T>& mgr(detail::getHandleManager<T>());
   mgr.update(index_, target);
}

} // namespace be

#endif
