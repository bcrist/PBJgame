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
/// \file   pbj/sw/resource_id.h
/// \author Benjamin Crist
///
/// \brief  pbj::sw::ResourceId class header.

#ifndef PBJ_SW_RESOURCE_ID_H_
#define PBJ_SW_RESOURCE_ID_H_

#include "be/id.h"
#include "pbj/_pbj.h"

namespace pbj {
namespace sw {

///////////////////////////////////////////////////////////////////////////////
/// \struct ResourceId   pbj/sw/resource_id.h "pbj/sw/resource_id.h"
///
/// \brief  Combines the Id of the Sandwich a resource is located in and
///         the Id of the resource itself.
/// \details A ResourceId represents a unique resource among all resources of
///         that type.  Resources in different Sandwiches may have the same
///         resource portion of their ResourceId.  The ResourceId contains no
///         information about what type of resource it is referring to.
class ResourceId
{
public:
   ResourceId();
   ResourceId(const Id& bed, const Id& asset);

   std::string to_string() const;

   bool operator==(const ResourceId& other) const;
   bool operator!=(const ResourceId& other) const;
   bool operator<(const ResourceId& other) const;
   bool operator>(const ResourceId& other) const;
   bool operator<=(const ResourceId& other) const;
   bool operator>=(const ResourceId& other) const;

   Id sandwich;     ///< Identifies the Sandwich this asset can be found in
   Id resource;     ///< Differentiates this resource from others of its type in its Sandwich.
};

std::ostream& operator<<(std::ostream& os, const ResourceId& asset_id);

} // namespace pbj::sw
} // namespace pbj

#include "pbj/sw/resource_id.inl"

#endif
