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
/// \file   be/handle.inl
/// \author Benjamin Crist
///
/// \brief  Implementations of be::Handle template functions.

#if !defined(BE_HANDLE_H_) && !defined(DOXYGEN)
#include "be/handle.h"
#elif !defined(BE_HANDLE_INL_)
#define BE_HANDLE_INL_

namespace be {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a handle in an invalidated state.
/// \details Any attempts to retrieve a pointer to the handled object will
///         result in \c nullptr and implicit conversion to bool will yield
///         false.
template <class T>
inline Handle<T>::Handle()
   : ConstHandle<T>()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a handle that references the same object as the handle
///         passed in.
/// \param  other The handle to copy.
template <class T>
inline Handle<T>::Handle(const Handle<T>& other)
   : ConstHandle<T>(other)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Assigns this handle to reference the same object referenced by
///         another handle.
/// \param  other The handle to copy.
template <class T>
inline Handle<T>& Handle<T>::operator=(const Handle<T>& other)
{
   check_ = other.check_;
   index_ = other.index_;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a pointer to the referenced object.
/// \details A handle that isn't referencing anything (for instance, one
///         created with the default constructor) will yield a \c nullptr.  A
///         handle which refers to an object that has been destroyed will also
///         return \c nullptr.
///
/// \return A pointer to the referenced object, or \c nullptr if the handle is
///         invalid.
template <class T>
inline T* Handle<T>::get() const
{
   if (check_ == 0)
      return nullptr;

   detail::HandleManager<T>& mgr(detail::getHandleManager<T>());

   if (index_ >= mgr.entries.size())
      return nullptr;

   const detail::HandleEntry<T>& entry(mgr.entries[index_]);

   if (entry.check != check_)
      return nullptr;

   return entry.target;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Shorthand for get()->
template <class T>
inline T* Handle<T>::operator->() const
{
   return get();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Shorthand for *get()
template <class T>
inline T& Handle<T>::operator*() const
{
   return *get();
}

} // namespace be

#endif
