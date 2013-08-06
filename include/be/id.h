///////////////////////////////////////////////////////////////////////////////
/// \file   be/id.h
/// \author Benjamin Crist
///
/// \brief  be::Id class header.

#ifndef BE_ID_H_
#define BE_ID_H_

#include "be/_be.h"

#include <cstdint>
#include <ostream>

#ifdef DOXYGEN

///////////////////////////////////////////////////////////////////////////////
/// \brief  Indicates that Id names should be remembered and hash collision
///         checking should be enabled.
/// \details If #BE_ID_NAMES_ENABLED and #BE_ID_NAMES_DISABLED are both defined
///         then #BE_ID_NAMES_ENABLED takes precedence.  If neither name is
///         explicitly defined, #BE_ID_NAMES_ENABLED will be defined only if
///         #DEBUG or #BE_BEDITOR is defined.
/// \ingroup ids
#define BE_ID_NAMES_ENABLED

///////////////////////////////////////////////////////////////////////////////
/// \brief  Indicates that Id names should not be remembered and hash collision
///         checking should be disabled.
/// \details Disabling these features will result in a slight performance
///         boost.
/// \ingroup ids
#define BE_ID_NAMES_DISABLED

#endif

#if (defined BE_BEDITOR || defined(DEBUG)) && !defined(BE_ID_NAMES_ENABLED) && !defined(BE_ID_NAMES_DISABLED)
#define BE_ID_NAMES_ENABLED
#endif

///////////////////////////////////////////////////////////////////////////////
/// \brief  The initial state used in the 64-bit FNV-1a hash algorithm.
/// \ingroup ids
#define BE_ID_FNV_OFFSET_BASIS 14695981039346656037ULL

///////////////////////////////////////////////////////////////////////////////
/// \brief  A large prime number used by the 64-bit FNV-1a hash algorithm.
/// \ingroup ids
#define BE_ID_FNV_PRIME 1099511628211ULL


namespace be {

///////////////////////////////////////////////////////////////////////////////
/// \class  Id   be/id.h "be/id.h"
///
/// \brief  Identifier object.
/// \details Id objects are used to efficiently represent game resources and
///         extensible engine types.  They can be used for map keys, event
///         types, etc.
///
///         Each ID consists of a 64-bit integer.  IDs with the same integer
///         value are considered equal.  Id objects can be constructed directly
///         from a numeric value or by hashing a string.  When hashing strings,
///         it is possible for collisions to occur (though it is extremely rare
///         due to the use of 64-bits of state), so the use of hashed strings
///         is best reserved for situations where the string value is known
///         prior to runtime (compile-time constants, asset names, etc.).
///
///         If #BE_ID_NAMES_ENABLED is defined, the string parameters used to
///         construct objects are remembered.  This makes debugging easier,
///         since string representations of the Id can include the original
///         text as well as the hashed value.  In addition, a warning is
///         emitted any time a hash collision occurs (two different strings
///         yield the same hash).  By default, #BE_ID_NAMES_ENABLED will be
///         defined if either #DEBUG or #BE_BEDITOR is defined and not
///         defined otherwise, but this behavior can be overridden by
///         explicitly defining either #BE_ID_NAMES_ENABLED or
///         #BE_ID_NAMES_DISABLED.
/// \ingroup ids
class Id
{
public:
   Id();
   explicit Id(uint64_t id);
   explicit Id(const std::string& name);
   Id(const Id& other);
   Id& operator=(const Id& other);

   std::string to_string() const;

   uint64_t value() const;

   bool operator==(const Id& other) const;
   bool operator!=(const Id& other) const;
   bool operator<(const Id& other) const;
   bool operator>(const Id& other) const;
   bool operator<=(const Id& other) const;
   bool operator>=(const Id& other) const;

private:
   uint64_t id_;
};

std::ostream& operator<<(std::ostream& os, const Id& id);

} // namespace be

#include "be/id.inl"

#endif
