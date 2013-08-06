///////////////////////////////////////////////////////////////////////////////
/// \file   be/id.inl
/// \author Benjamin Crist
///
/// \brief  Implementations of be::Id template functions.

#if !defined(BE_ID_H_) && !defined(DOXYGEN)
#include "be/id.h"
#elif !defined(BE_ID_INL_)
#define BE_ID_INL_

///////////////////////////////////////////////////////////////////////////////
/// \brief  \c std::hash specialization for utilizing Id objects in
///         \c std::unordered_set and \c std::unordered_map containers.
/// \ingroup ids
template<>
struct std::hash<be::Id>
{
public:
   ////////////////////////////////////////////////////////////////////////////
   /// \brief  Calculates the hashcode of the provided Id.
   /// \details In 64-bit builds, the Id's value is simply used directly as the
   ///         hashcode.  In 32-bit builds, the upper half of the value is
   ///         XORed with the lower 32 bits to yield a 32-bit \c size_t result.
   /// \param id The Id to hash.
   /// \return A hashcode suitable for use in hashtable-based data structures.
   size_t operator()(const be::Id& id) const
   {
      size_t value = static_cast<size_t>(id.value());

      static_assert(sizeof(size_t) >= 4, "size_t must be at least 32 bits!");
      if (sizeof(size_t) < sizeof(uint64_t))
         value ^= id.value() >> (8 * sizeof(size_t));

      return value;
   }
};

#endif
