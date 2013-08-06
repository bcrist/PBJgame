///////////////////////////////////////////////////////////////////////////////
/// \file   be/id.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of be::Id functions

#include "be/id.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#ifdef BE_ID_NAMES_ENABLED
#include <unordered_map>
#include <mutex>
#endif

namespace be {
namespace {

#ifdef BE_ID_NAMES_ENABLED

// Retrieves a map of Id values to strings used to generate them.
std::unordered_map<uint64_t, std::string>& getNames()
{
   static std::unordered_map<uint64_t, std::string> names;
   return names;
}

// Retrieves a mutex that protects the map above.
std::mutex& getNamesMutex()
{
   static std::mutex mutex;
   return mutex;
}

#endif // BE_ID_NAMES_ENABLED


// Calculates 64-bit FNV-1a hash of provided name.
uint64_t hash(const std::string& name)
{
   uint64_t hash(BE_ID_FNV_OFFSET_BASIS);
   for (auto i(name.begin()), end(name.end()); i != end; ++i)
      hash = (hash ^ static_cast<uint8_t>(*i)) * BE_ID_FNV_PRIME;

#ifdef BE_ID_NAMES_ENABLED
   std::unordered_map<uint64_t, std::string>& names = getNames();
   std::mutex& mutex = getNamesMutex();
   std::lock_guard<std::mutex> lock(mutex);
   auto i(names.find(hash));
   if (i != names.end())
   {
      // Check for collision with any previously hashed name
      if (i->second != name)
      {
         // Hash collision: Print a warning
         std::ostringstream oss;
         oss << "ID hash collision detected!" << BE_LOG_NL
             << "    Hash: " << std::hex << std::setfill('0') << std::setw(16) << hash << BE_LOG_NL
             << "Existing: " << i->second << BE_LOG_NL
             << " Current: " << name << BE_LOG_END;
         BE_LOG(VNotice) << oss;
      }
   }
   else
   {
      // Save this name/hash for future checks
      names[hash] = name;          
   }
#endif // BE_ID_NAMES_ENABLED

   return hash;
}

} // namespace be::(annon)


///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new Id object which has the same value as if it were 
///         constructed from a zero-length string.
Id::Id()
   : id_(BE_ID_FNV_OFFSET_BASIS)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new Id object which has the exact integer value
///         specified.
///
/// \param  id The numeric value for the new Id.
Id::Id(uint64_t id)
   : id_(id)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new Id object by hashing a string value.
///
/// \details The hash function used is a Fowler-Noll-Vo variant.
///         (64-bit FNV-1a)
///
/// \param  name The string to hash to generate the numeric value for the Id.
Id::Id(const std::string& name)
   : id_(hash(name))
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new Id object which has the same value as an existing
///         Id. (copy constructor)
///
/// \param  other The Id whose value we will copy.
Id::Id(const Id& other)
   : id_(other.id_)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Assigns an Id the same value as another Id object.
///
/// \param  other The Id whose value we will copy.
/// \return *this
Id& Id::operator=(const Id& other)
{
   id_ = other.id_;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns a string representation of this Id.
///
/// \details The string will be at least seventeen characters long, beginning
///         with a '#' and followed by the value of the Id in 16 digits of
///         zero-padded hexadecimal.  The case of hexadecimal digits A-F should
///         not be relied upon.
///
///         If #BE_ID_NAMES_ENABLED is defined and an Id with this value has
///         been constructed by hashing a string, the hex value is followed by
///         a ':' and the string that was hashed like so:
///         <tt>#8ae793af1a1dc82a:Aduka!</tt>.
///
/// \note   Even if an object was created directly using a numeric value, it
///         may still display a textual name as well if another object was
///         created from a string that hashes to the same value.  For instance:
///
/// \code
///    be::Id id1(0x90285684421F9857);
///    std::cout << id1 << std::endl;  // outputs "#90285684421f9857"
///
///    be::Id id2("asdf");
///    std::cout << id1 << std::endl;  // outputs "#90285684421f9857:asdf"
///    std::cout << id2 << std::endl;  // outputs "#90285684421f9857:asdf"
/// \endcode
///
/// \return A string representation of the Id.
std::string Id::to_string() const
{
   std::ostringstream oss;
   oss << '#' << std::hex << std::setfill('0') << std::setw(16) << id_;

#ifdef BE_ID_NAMES_ENABLED
   std::unordered_map<uint64_t, std::string>& names = getNames();
   std::mutex& mutex = getNamesMutex();
   std::lock_guard<std::mutex> lock(mutex);
   auto i(names.find(id_));
   if (i != names.end())
      oss << ':' << i->second;
#endif

   return oss.str();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the numeric value of this object.
///
/// \return The 64-bit numeric value of this Id. 
uint64_t Id::value() const
{
   return id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this Id with another to see if they have the same value.
///
/// \param  other The Id to compate to this one.
/// \return \c true if both Ids have the same numeric value.
bool Id::operator==(const Id& other) const
{
   return id_ == other.id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this Id with another to see if they have different values.
///
/// \param  other The Id to compate to this one.
/// \return \c true if the Ids have different numeric values.
bool Id::operator!=(const Id& other) const
{
   return id_ != other.id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this Id with another to see if it has a smaller value
///         than the other.
///
/// \param  other The Id to compate to this one.
/// \return \c true if this Id's numeric value is less than other's.
bool Id::operator<(const Id& other) const
{
   return id_ < other.id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this Id with another to see if it has a larger value
///         than the other.
///
/// \param  other The Id to compate to this one.
/// \return \c true if this Id's numeric value is greater than other's.
bool Id::operator>(const Id& other) const
{
   return id_ > other.id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this Id with another to see if its value is not larger
///         than other's.
///
/// \param  other The Id to compate to this one.
/// \return \c true if this Id's numeric value is not larger than other's.
bool Id::operator<=(const Id& other) const
{
   return id_ <= other.id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares this Id with another to see if its value is not smaller
///         than other's.
///
/// \param  other The Id to compate to this one.
/// \return \c true if this Id's numeric value is not smaller than other's.
bool Id::operator>=(const Id& other) const
{
   return id_ >= other.id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Outputs a string representation of an Id object to the provided
///         stream.
///
/// \details The format used is the same as that used by Id::to_string().
///
/// \param  os The \c std::ostream to output to.
/// \param  id The Id to output.
/// \return The same \c std::ostream passed in (allows for operator chaining).
std::ostream& operator<<(std::ostream& os, const Id& id)
{
   return os << id.to_string();
}

} // namespace be
