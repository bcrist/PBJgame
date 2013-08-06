///////////////////////////////////////////////////////////////////////////////
/// \file   be/const_handle.h
/// \author Benjamin Crist
///
/// \brief  be::ConstHandle class header.

#ifndef BE_CONST_HANDLE_H_
#define BE_CONST_HANDLE_H_

#include "be/detail/handle_manager.h"

#include <ostream>

namespace be {

///////////////////////////////////////////////////////////////////////////////
/// \class  ConstHandle   be/const_handle.h "be/const_handle.h"
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
///         A ConstHandle behaves similarly to a pointer to const data.  Handle
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
class ConstHandle
{
public:
   ConstHandle();
   ConstHandle(const ConstHandle<T>& other);
   ConstHandle<T>& operator=(const ConstHandle<T>& other);

   operator bool() const;

   const T* get() const;
   const T* operator->() const;
   const T& operator*() const;

   uint32_t getInternalIndex() const;

   bool operator==(const ConstHandle<T>& other) const;
   bool operator!=(const ConstHandle<T>& other) const;

protected:
   uint32_t index_;  ///< An index which tells the HandleManager the location of the handled object's address
   uint32_t check_;  ///< A value used by the HandleManager to ensure that the object at index_ is the same as it was when this handle was created.
};

template <class T>
std::ostream& operator<<(std::ostream& os, const be::ConstHandle<T>& handle);

template <class T>
size_t getHandledObjectCount();

} // namespace be

#include "be/const_handle.inl"

#endif
