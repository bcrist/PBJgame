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
/// \file   be/asset_id.h
/// \author Benjamin Crist
///
/// \brief  be::AssetId class header.

#ifndef BE_ASSET_ID_H_
#define BE_ASSET_ID_H_

#include "be/id.h"

namespace be {

///////////////////////////////////////////////////////////////////////////////
/// \struct AssetId   be/asset_id.h "be/asset_id.h"
///
/// \brief  Combines the ID of the \ref bed an asset is located in and the ID
///         of the asset itself.
/// \details An AssetId represents a unique asset among all assets of that
///         type.  Assets in different beds may have the same asset portion
///         of their AssetId.  The AssetId contains no information about what
///         type of asset it is referring to.
/// \ingroup ids
struct AssetId
{
   AssetId();
   AssetId(const Id& bed, const Id& asset);

   std::string to_string() const;

   bool operator==(const AssetId& other) const;
   bool operator!=(const AssetId& other) const;
   bool operator<(const AssetId& other) const;
   bool operator>(const AssetId& other) const;
   bool operator<=(const AssetId& other) const;
   bool operator>=(const AssetId& other) const;

   Id bed;     ///< Identifies the \ref bed this asset can be found in
   Id asset;   ///< Differentiates this asset from others of its type in its bed.
};

std::ostream& operator<<(std::ostream& os, const AssetId& asset_id);

} // namespace be

#include "be/asset_id.inl"

#endif
