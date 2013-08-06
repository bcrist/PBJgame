///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/sw/resource_id.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::sw::ResourceId functions.

#include "pbj/sw/resource_id.h"

#include <sstream>

namespace pbj {
namespace sw {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new ResourceId object whose sandwich and resource
///         fields are default-constructed.
ResourceId::ResourceId()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new ResourceId from two Id objects.
///
/// \param  sandwich The sandwich portion of the ResourceId.
/// \param  resource The resource portion of the ResourceId.
ResourceId::ResourceId(const Id& sandwich, const Id& resource)
   : sandwich(sandwich),
     resource(resource)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns a string representation of this ResourceId.
///
/// \details The ResourceId's sandwich and resource fields are converted to
///         string, separated by a pipe ('|') character and surrounded by
///         braces.  For example:
///         <tt>{#0000000000000000|#0000000000000000}</tt>
///
/// \return A string representation of the ResourceId.
std::string ResourceId::to_string() const
{
   std::ostringstream oss;
   oss << '{' << sandwich.to_string() << '|' << resource.to_string() << '}';

   return oss.str();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this ResourceId with another to see if they reference the
///         same thing.
///
/// \details The sandwich and resource fields must both match to be considered
///         equal.
///
/// \note   Since ResourceIds don't have any information about the type of
///         objects they describe, ResourceIds should only be compared to
///         others representing the same type of object.
///
/// \param  other The ResourceId to compare this one to.
/// \return \c true if both ResourceIds have the same value.
bool ResourceId::operator==(const ResourceId& other) const
{
   return sandwich == other.sandwich && resource == other.resource;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this ResourceId with another to see if they have reference
///         different things.
///
/// \details At least one of either the sandwich or resource fields must differ
///         for the ResourceIds to be considered not equal.
///
/// \note   Since ResourceIds don't have any information about the type of
///         objects they describe, ResourceIds should only be compared to
///         others representing the same type of object.
///
/// \param  other The ResourceId to compare this one to.
/// \return \c true if the ResourceIds have different values.
bool ResourceId::operator!=(const ResourceId& other) const
{
   return !(*this == other);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this ResourceId with another to see if it has a smaller
///         value than the other.
///
/// \note   Since ResourceIds don't have any information about the type of
///         objects they describe, ResourceIds should only be compared to
///         others representing the same type of object.
///
/// \param  other The ResourceId to compare this one to.
/// \return \c true if this ResourceId's sandwich is 'less than' other's, or
///         both sandwichs are the same but this ResourceId's resource field is
///         'less than' other's.
bool ResourceId::operator<(const ResourceId& other) const
{
   return sandwich < other.sandwich ||
          sandwich == other.sandwich && resource < other.resource;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this ResourceId with another to see if it has a larger
///         value than the other.
///
/// \note   Since ResourceIds don't have any information about the type of
///         objects they describe, ResourceIds should only be compared to
///         others representing the same type of object.
///
/// \param  other The ResourceId to compare this one to.
/// \return \c true if this ResourceId's sandwich is 'greater than' other's, or
///         both sandwichs are the same but this ResourceId's resource field is
///         'greater than' other's.
bool ResourceId::operator>(const ResourceId& other) const
{
   return sandwich > other.sandwich ||
          sandwich == other.sandwich && resource > other.resource;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this ResourceId with another to see if it has a not larger
///         value than the other.
///
/// \note   Since ResourceIds don't have any information about the type of
///         objects they describe, ResourceIds should only be compared to
///         others representing the same type of object.
///
/// \param  other The ResourceId to compare this one to.
/// \return \c true if this ResourceId's sandwich is not 'greater than'
///         other's, or both sandwichs are the same but this ResourceId's
///         resource field is not 'greater than' other's.
bool ResourceId::operator<=(const ResourceId& other) const
{
   return !(*this > other);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this ResourceId with another to see if it has a not
///         smaller value than the other.
///
/// \note   Since ResourceIds don't have any information about the type of
///         objects they describe, ResourceIds should only be compared to
///         others representing the same type of object.
///
/// \param  other The ResourceId to compare this one to.
/// \return \c true if this ResourceId's sandwich is not 'less than' other's,
///         or both sandwichs are the same but this ResourceId's resource
///         field is not 'less than' other's.
bool ResourceId::operator>=(const ResourceId& other) const
{
   return !(*this < other);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Outputs a string representation of an ResourceId object to the
///         provided stream.
///
/// \details The format used is the same as that used by
///         ResourceId::to_string().
///
/// \param  os The \c std::ostream to output to.
/// \param  resource_id The ResourceId object to output.
/// \return The same \c std::ostream that was passed in, to facilitate operator
///         chaining.
std::ostream& operator<<(std::ostream& os, const ResourceId& resource_id)
{
   return os << resource_id.to_string();
}

} // namespace pbj::sw
} // namespace pbj
