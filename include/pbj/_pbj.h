///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/_pbj.h
/// \author Benjamin Crist
///
/// \brief  Base PBJgame include file.

#ifndef PBJ_PBJ_H_
#define PBJ_PBJ_H_

#ifdef DOXYGEN

///////////////////////////////////////////////////////////////////////////////
/// \brief Indicates that the unit tests and testing framework should be built
///        instead of the game/editor.
///
/// \details PBJ_TEST implies #BE_TEST
#define PBJ_TEST

///////////////////////////////////////////////////////////////////////////////
/// \brief  Defined when the engine is running in editor mode.
/// \details In this mode, various additional features are enabled which make
///         editing game resources easier.
/// 
///         PBJ_EDITOR implies #BE_BEDITOR.
#define PBJ_EDITOR

#else

#if defined(PBJ_TEST) && !defined(BE_TEST)
#define BE_TEST
#endif

#if defined(PBJ_EDITOR) && !defined(BE_BEDITOR)
#define BE_BEDITOR
#endif

#include "be/_be.h"

#include <functional>
// make sure size_t, ptrdiff_t, int64_t, etc. are defined
#include <cstddef>
#include <cstdint>

#endif

///////////////////////////////////////////////////////////////////////////////
/// \brief  The PBJgame major version number.
#define PBJ_VERSION_MAJOR 0

///////////////////////////////////////////////////////////////////////////////
/// \brief  The PBJgame minor version number.
#define PBJ_VERSION_MINOR 1

///////////////////////////////////////////////////////////////////////////////
/// \brief  The PBJgame copyright string
#define PBJ_COPYRIGHT "Copyright (c) 2013 PBJ^2 Productions"


namespace be {

class Id;

} // namespace be


///////////////////////////////////////////////////////////////////////////////
/// \brief  The root PBJgame namespace.
namespace pbj {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Implementation details used by classes in \ref pbj.
namespace detail { }


////////////////////////////////////////////////////////////////////////////////
/// \brief	Namespace for drawing.
namespace gfx { typedef std::function<void()> ComponentCallback; }

///////////////////////////////////////////////////////////////////////////////
/// \brief  Alias of be::bed.
namespace db = be::bed;

///////////////////////////////////////////////////////////////////////////////
/// \brief  Defines verbosity levels used by the #PBJ_LOG and the \ref be
///         verbosity functions.
///
/// \sa     be::Verbosity
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


// define fixed-width numeric types.
typedef int8_t I8;      ///< 8 bit signed integer type.
typedef uint8_t U8;     ///< 8 bit unsigned integer type.
typedef int16_t I16;    ///< 16 bit signed integer type.
typedef uint16_t U16;   ///< 16 bit unsigned integer type.
typedef int32_t I32;    ///< 32 bit signed integer type.
typedef uint32_t U32;   ///< 32 bit unsigned integer type.
typedef float F32;      ///< 32 bit floating point type.
typedef int64_t I64;    ///< 64 bit signed integer type.
typedef uint64_t U64;   ///< 64 bit unsigned integer type.
typedef double F64;     ///< 64 bit floating point type.

// alias \ref be classes
typedef be::Id Id;

} // namespace pbj

///////////////////////////////////////////////////////////////////////////////
/// \brief  The \c std::ostream to use for logging.
#define PBJ_LOG_STREAM BE_LOG_STREAM

///////////////////////////////////////////////////////////////////////////////
/// \brief  Writes a message to the log.
///
/// \details Wraps the #BE_LOG macro.
///
/// \sa     BE_LOG
#define PBJ_LOG(level) BE_LOG(level)

///////////////////////////////////////////////////////////////////////////////
/// \brief  Begins a continuation line for #PBJ_LOG messages.
///
/// \sa     BE_LOG_NL
#define PBJ_LOG_NL BE_LOG_NL

///////////////////////////////////////////////////////////////////////////////
/// \brief  Ends a #PBJ_LOG message
///
/// \sa     BE_LOG_END
#define PBJ_LOG_END BE_LOG_END
#endif
