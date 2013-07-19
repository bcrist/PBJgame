// Copyright (c) 2013 PBJ^2 Productions
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
