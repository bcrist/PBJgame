///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/sw/resource_id.inl
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::sw::ResourceId template functions.

#if !defined(PBJ_SW_RESOURCE_ID_H_) && !defined(DOXYGEN)
#include "pbj/sw/resource_id.h"
#elif !defined(PBJ_SW_RESOURCE_ID_INL_)
#define PBJ_SW_RESOURCE_ID_INL_

///////////////////////////////////////////////////////////////////////////////
/// \brief  \c std::hash specialization for utilizing ResourceId objects in
///         \c std::unordered_set and \c std::unordered_map containers.
template<>
struct std::hash<pbj::sw::ResourceId>
{
public:

    ////////////////////////////////////////////////////////////////////////////
    /// \brief  Calculates the hashcode of the provided ResourceId.
    /// \details Just XORs the \c std::hash values of the two Ids that make up
    ///         the ResourceId.
    /// \param  id The ResourceId to hash.
    /// \return A hashcode suitable for use in hashtable-based data structures.
    size_t operator()(const pbj::sw::ResourceId& id) const
    {
        std::hash<be::Id> hash_id;

        return hash_id(id.sandwich) ^ hash_id(id.resource);
    }
};

#endif
