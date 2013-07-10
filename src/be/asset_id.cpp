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
/// \file   be/asset_id.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of be::AssetId functions.

#include "be/asset_id.h"

#include <sstream>

namespace be {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new AssetId object whose bed and asset fields are
///         default-constructed.
AssetId::AssetId()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new AssetId from two Id objects.
///
/// \param  bed The bed portion of the AssetId.
/// \param  asset The asset portion of the AssetId.
AssetId::AssetId(const Id& bed, const Id& asset)
   : bed(bed),
     asset(asset)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns a string representation of this AssetId.
///
/// \details The AssetId's bed and asset fields are converted to string,
///         separated by a pipe ('|') character and surrounded by braces.  For
///         example: <tt>{#0000000000000000|#0000000000000000}</tt>
///
/// \return A string representation of the AssetId.
std::string AssetId::to_string() const
{
   std::ostringstream oss;
   oss << '{' << bed.to_string() << '|' << asset.to_string() << '}';

   return oss.str();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this AssetId with another to see if they reference the
///         same thing.
///
/// \details The bed and asset fields must both match to be considered equal.
///
/// \note   Since AssetIds don't have any information about the type of objects
///         they describe, AssetIds should only be compared to others
///         representing the same type of object.
///
/// \param  other The AssetId to compare this one to.
/// \return \c true if both AssetIds have the same value.
bool AssetId::operator==(const AssetId& other) const
{
   return bed == other.bed && asset == other.asset;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this AssetId with another to see if they have reference
///         different things.
///
/// \details At least one of either the bed or asset fields must differ for 
///         the AssetIds to be considered not equal.
///
/// \note   Since AssetIds don't have any information about the type of objects
///         they describe, AssetIds should only be compared to others
///         representing the same type of object.
///
/// \param  other The AssetId to compare this one to.
/// \return \c true if the AssetIds have different values.
bool AssetId::operator!=(const AssetId& other) const
{
   return !(*this == other);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this AssetId with another to see if it has a smaller
///         value than the other.
///
/// \note   Since AssetIds don't have any information about the type of objects
///         they describe, AssetIds should only be compared to others
///         representing the same type of object.
///
/// \param  other The AssetId to compare this one to.
/// \return \c true if this AssetId's bed is 'less than' other's, or both
///         beds are the same but this AssetId's asset field is 'less than'
///         other's.
bool AssetId::operator<(const AssetId& other) const
{
   return bed < other.bed || bed == other.bed && asset < other.asset;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this AssetId with another to see if it has a larger
///         value than the other.
///
/// \note   Since AssetIds don't have any information about the type of objects
///         they describe, AssetIds should only be compared to others
///         representing the same type of object.
///
/// \param  other The AssetId to compare this one to.
/// \return \c true if this AssetId's bed is 'greater than' other's, or both
///         beds are the same but this AssetId's asset field is 'greater than'
///         other's.
bool AssetId::operator>(const AssetId& other) const
{
   return bed > other.bed || bed == other.bed && asset > other.asset;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this AssetId with another to see if it has a not larger
///         value than the other.
///
/// \note   Since AssetIds don't have any information about the type of objects
///         they describe, AssetIds should only be compared to others
///         representing the same type of object.
///
/// \param  other The AssetId to compare this one to.
/// \return \c true if this AssetId's bed is not 'greater than' other's, or
///         both beds are the same but this AssetId's asset field is not
///         'greater than' other's.
bool AssetId::operator<=(const AssetId& other) const
{
   return !(*this > other);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this AssetId with another to see if it has a not smaller
///         value than the other.
///
/// \note   Since AssetIds don't have any information about the type of objects
///         they describe, AssetIds should only be compared to others
///         representing the same type of object.
///
/// \param  other The AssetId to compare this one to.
/// \return \c true if this AssetId's bed is not 'less than' other's, or both
///         beds are the same but this AssetId's asset field is not 'less than'
///         other's.
bool AssetId::operator>=(const AssetId& other) const
{
   return !(*this < other);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Outputs a string representation of an AssetId object to the
///         provided stream.
///
/// \details The format used is the same as that used by AssetId::to_string().
///
/// \param  os The \c std::ostream to output to.
/// \param  asset_id The AssetId object to output.
/// \return The same \c std::ostream that was passed in, to facilitate operator
///         chaining.
std::ostream& operator<<(std::ostream& os, const be::AssetId& asset_id)
{
   return os << asset_id.to_string();
}

} // namespace be
