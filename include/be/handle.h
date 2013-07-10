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
/// \file   be/handle.h
/// \author Benjamin Crist
///
/// \brief  be::Handle class header.

#ifndef BE_HANDLE_H_
#define BE_HANDLE_H_

#include "be/const_handle.h"

namespace be {

///////////////////////////////////////////////////////////////////////////////
/// \class  Handle  be/handle.h "be/handle.h"
///
/// \brief  A pointer-like object which remains usable when its target is
///         destroyed.
/// \details Handles can be used to uniquely identify an object in memory.
///         Unlike plain pointers or references, if the handle's target is
///         destroyed or invalidated, the handle remains usable (if the object
///         is no longer accessible, it will resolve to null, rather than
///         causing illegal access errors).  Note that this means handles
///         should not be used as keys in map or set data structures which
///         assume key objects remain constant as long as they exist in the
///         container.  If a handle were used as a key (using a comparator
///         based on the value returned by get()), and the handled object were
///         destroyed, it would likely corrupt the rest of the container.
///
///         A Handle behaves similarly to a pointer to non-const data.  Handle
///         objects can be converted into ConstHandle objects implicitly (but
///         the reverse is not possible).  Neither Handle nor ConstHandle
///         provides any facility for setting or changing the target of a
///         handle.  For that, a SourceHandle is required.
///
/// \note   Due to the C++ static deinitialization order fiasco, static objects
///         should not access any handles during deinitialization, though the
///         destruction of handle objects themselves (other than SourceHandles)
///         will not cause problems at static deinitialization time.
/// \tparam T The type of the object to be handled.
/// \ingroup handles
template <class T>
class Handle : public ConstHandle<T>
{
public:
   Handle();
   Handle(const Handle<T>& other);
   Handle<T>& operator=(const Handle<T>& other);

   T* get() const;
   T* operator->() const;
   T& operator*() const;
};

} // namespace be

#include "be/handle.inl"

#endif
