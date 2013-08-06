///////////////////////////////////////////////////////////////////////////////
/// \file   be/_be.h
/// \author Benjamin Crist
///
/// \brief  Base bengine include file.

#ifndef BE_BE_H_
#define BE_BE_H_

#ifdef DOXYGEN

///////////////////////////////////////////////////////////////////////////////
/// \brief  Indicates that the engine was built in debug mode.
/// \details This has various effects, such as enabling assertion checks,
///         standard library iterator checks, disabling error log redirection,
///         and enabling memory leak detection on Windows.
#define DEBUG

///////////////////////////////////////////////////////////////////////////////
/// \brief  Indicates that the engine was <b>not</b> built in debug mode.
/// \sa     DEBUG
#define NDEBUG

///////////////////////////////////////////////////////////////////////////////
/// \brief Indicates that the unit tests and testing framework should be built
///        instead of the game/editor.
#define BE_TEST

///////////////////////////////////////////////////////////////////////////////
/// \brief  #BE_BEDITOR is defined when bengine is running in editor mode.
/// \details In this mode, various additional features are enabled which make
///         editing game resources easier, such as Id names and Id collision
///         checking.
#define BE_BEDITOR

#else

#define _USE_MATH_DEFINES
#include <cmath>
#include <string>

#if defined(_MSC_VER) && defined(DEBUG) && !defined(new)
#include <crtdbg.h>
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#define BE_CRT_BUILD
#endif

#ifdef _CRT_SECURE_NO_WARNINGS
#undef _CRT_SECURE_NO_WARNINGS
#endif

#endif

///////////////////////////////////////////////////////////////////////////////
/// \brief  The bengine major version number.
#define BE_VERSION_MAJOR 0

///////////////////////////////////////////////////////////////////////////////
/// \brief  The bengine minor version number.
#define BE_VERSION_MINOR 1

///////////////////////////////////////////////////////////////////////////////
/// \brief  The bengine copyright string
#define BE_COPYRIGHT "Copyright (c) 2013 Benjamin Crist"

///////////////////////////////////////////////////////////////////////////////
/// \brief  The root <b>BE</b>ngine namespace.
namespace be {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Implementation details used by classes in \ref be.
namespace detail { }

///////////////////////////////////////////////////////////////////////////////
/// \brief  <b>BE</b>ngine <b>D</b>atabases.
/// \details Allows the accessing of game data stored in SQLite3 databases. 
namespace bed {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Implementation details used by classes in be::bed.
namespace detail { }

} // namespace be::bed

///////////////////////////////////////////////////////////////////////////////
/// \brief  Basic utility functions which don't really belong anywhere else.
namespace util { }



///////////////////////////////////////////////////////////////////////////////
/// \brief  Defines verbosity levels used by the #BE_LOG and the verbosity
///         functions.
enum Verbosity
{   
   VError = 1,                      ///< Used when logging a serious (generally fatal) error.
   VWarning = 2,                    ///< Used when logging a potential problem.
   VNotice = 4,                     ///< Used when logging an unexpected situation that likely won't cause any problems.
   VInfo = 8,                       ///< Used when logging a message which is a result of normal operation, but still might be interesting.

   VErrors = 1,                     ///< Used with setVerbosity() to specify that only \ref VError messages should be output.
   VErrorsAndWarnings = 3,          ///< Used with setVerbosity() to specify that \ref VError and \ref VWarning messages should be output.
   VErrorsWarningsAndNotices = 7,   ///< Used with setVerbosity() to specify that \ref VError, \ref VWarning, and \ref VNotice messages should be output, but not \ref VInfo messages.
   VAll = 0x0FFFFFFF                ///< Used with setVerbosity() to specify that all messages should be output.
};

void setVerbosity(int verbosity);
int getVerbosity();
bool checkVerbosity(int verbosity);
std::string getCurrentTimeString();
const char* getVerbosityLevel(int level);

} // namespace be

///////////////////////////////////////////////////////////////////////////////
/// \brief  The \c std::ostream to use for logging.
#define BE_LOG_STREAM std::cerr

///////////////////////////////////////////////////////////////////////////////
/// \brief  Writes a message to the log.
///
/// \details If the current verbosity includes the specified level, outputs a
///         message to #BE_LOG_STREAM using the following format:
///         <tt>"YYYY-MM-DD HH:MM:SS file.cpp(line) Level: "</tt>
///
///         The expression evaluates to #BE_LOG_STREAM so additional stream
///         insertion operators can be appended to the line like so:
///
/// \code   BE_LOG(VERBOSITY_NOTICE) << "Something interesting." << BE_LOG_END;
/// \endcode
/// \sa     be::Verbosity
/// \sa     BE_LOG_NL
/// \sa     BE_LOG_END
#define BE_LOG(level) if (be::checkVerbosity(level))          \
   BE_LOG_STREAM << be::getCurrentTimeString()                \
                 << " " << __FILE__ << "(" << __LINE__ << ")" \
                 << " " << be::getVerbosityLevel(level)       \
                 << ": "

///////////////////////////////////////////////////////////////////////////////
/// \brief  Begins a continuation line for #BE_LOG messages.
#define BE_LOG_NL std::endl << "                       "

///////////////////////////////////////////////////////////////////////////////
/// \brief  Ends a #BE_LOG message
#define BE_LOG_END std::endl

#endif
