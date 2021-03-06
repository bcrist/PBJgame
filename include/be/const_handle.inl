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
/// \file   be/const_handle.inl
/// \author Benjamin Crist
///
/// \brief  Implementations of be::ConstHandle template functions.

#if !defined(BE_CONST_HANDLE_H_) && !defined(DOXYGEN)
#include "be/const_handle.h"
#elif !defined(BE_CONST_HANDLE_INL_)
#define BE_CONST_HANDLE_INL_

namespace be {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a handle in an invalidated state.
/// \details Any attempts to retrieve a pointer to the handled object will
///         result in \c nullptr and implicit conversion to bool will yield
///         false.
template <class T>
inline ConstHandle<T>::ConstHandle()
   : index_(0),
     check_(0)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a handle that references the same object as the handle
///         passed in.
/// \param  other The handle to copy.
template <class T>
inline ConstHandle<T>::ConstHandle(const ConstHandle<T>& other)
   : index_(other.index_),
     check_(other.check_)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Assigns this handle to reference the same object referenced by
///         another handle.
/// \param  other The handle to copy.
template <class T>
inline ConstHandle<T>& ConstHandle<T>::operator=(const ConstHandle<T>& other)
{
   index_ = other.index_;
   check_ = other.check_;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Determines if this handle refers to a valid object.
/// \return \c true if get() would return a non-null pointer.
template <class T>
inline ConstHandle<T>::operator bool() const
{
   if (check_ == 0)     // technically any check_ where check_ & 0x7FFFFFFF == true is invalid, but
   {                    // those values are never stored in ConstHandle, only HandleManager
      return false;
   }

   detail::HandleManager<T>& mgr(detail::getHandleManager<T>());

   if (index_ >= mgr.entries.size())
      return false;

   if (mgr.entries[index_].check != check_)
      return false;

   return true;
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
inline const T* ConstHandle<T>::get() const
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
inline const T* ConstHandle<T>::operator->() const
{
   return get();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Shorthand for *get()
template <class T>
inline const T& ConstHandle<T>::operator*() const
{
   return *get();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this handle with another to see if they reference the same
///         target.
/// \details Two handles which once referenced different objects but are both
///         now invalidated will be considered equal.
/// \return true if get() == other.get().
template <class T>
inline bool ConstHandle<T>::operator==(const ConstHandle<T>& other) const
{
   return get() == other.get();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this handle with another to see if they reference
///         different targets.
/// \details Two handles which once referenced different objects but are both
///         now invalidated will not be considered different.
/// \return true if get() != other.get().
template <class T>
inline bool ConstHandle<T>::operator!=(const ConstHandle<T>& other) const
{
   return get() != other.get();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Outputs the address of the object referenced by the handle passed
///         in.
/// \details Equivalent to <tt>os << handle.get()</tt>
/// \param  os The \c std::ostream to output to.
/// \param  handle The handle to output.
/// \return The \c std::ostream provided (for chaining).
/// \ingroup handles
template <class T>
inline std::ostream& operator<<(std::ostream& os, const ConstHandle<T>& handle)
{
   return os << handle.get();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the current number of handled objects of the template type.
/// \details Essentially, this is the number of SourceHandle<T> objects that
///         exist at the moment.
/// \tparam T The type of objects to count.
/// \return The number of unique valid handled objects of type T.
/// \ingroup handles
template <class T>
inline size_t getHandledObjectCount()
{
   detail::HandleManager<T>& mgr(detail::getHandleManager<T>());
   return mgr.size;
}

} // namespace be

#endif
