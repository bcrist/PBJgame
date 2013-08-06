///////////////////////////////////////////////////////////////////////////////
/// \file   be/verbosity.cpp
/// \author Benjamin Crist
///
/// \brief  Functions related to console logging operations.

#include "be/_be.h"

#include <iostream>
#include <ctime>

namespace be {
namespace {

int& getVerbosityRef()
{
#ifdef DEBUG
   static int v = -1;
#else
   static int v = VWarning;
#endif

   return v;
}

} // namespace be::(anon)

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the current verbosity level to the value provided.
/// \param  verbosity The verbbosity level to use from now on.
/// \sa     Verbosity
void setVerbosity(int verbosity)
{
   getVerbosityRef() = verbosity;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the current verbosity level.
/// \sa     Verbosity
int getVerbosity()
{
   return getVerbosityRef();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Checks if the provided verbosity level should result in a message
///         being logged given the current verbosity level.
///
/// \param  verbosity The verbosity level to check against the current level.
/// \return \c true if getVerbosity() & verbosity == verbosity.
/// \sa     Verbosity
/// \sa     setVerbosity()
bool checkVerbosity(int verbosity)
{
   return (getVerbosityRef() & verbosity) == verbosity;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Gets the current system time as a string.
/// 
/// \details Generates a string representing the current system time and date
///         using the system's locale and formated like
///         <tt>"YYYY-MM-DD HH:MM:SS"</tt>.  Primarily used for logging error
///         messages using the #BE_LOG macro.
/// \return The generated string.
std::string getCurrentTimeString()
{
   time_t raw_time;
   const int buf_size = 20;
   char buf[buf_size];

   time(&raw_time);

#ifdef _MSC_VER
   tm local_;
   localtime_s(&local_, &raw_time);
   tm* local = &local_;
#else
   tm* local = localtime(&raw_time);
#endif

   strftime(buf, buf_size, "%Y-%m-%d %H:%M:%S", local);

   return buf;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Gets a string representation of a verbosity level.
/// \param  level the verbosity level we are interested in.
/// \return A C-string describing the type of message represented by the given
///         \ref Verbosity level, or "Unknown" if the verbosity level is not
///         recognized.
const char* getVerbosityLevel(int level)
{
   if ((level & VError) == VError)
      return "Error";

   if ((level & VWarning) == VWarning)
      return "Warning";

   if ((level & VNotice) == VNotice)
      return "Notice";

   if ((level & VInfo) == VInfo)
      return "Info";

   return "Unknown";
}

} // namespace be
