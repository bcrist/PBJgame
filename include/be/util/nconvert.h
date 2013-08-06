///////////////////////////////////////////////////////////////////////////////
/// \file   be/util/nconvert.h
/// \author Benjamin Crist
///
/// \brief  Utility functions for converting numbers between different forms.

#ifndef BE_UTIL_NCONVERT_H_
#define BE_UTIL_NCONVERT_H_
#include "be/_be.h"

#include <cstdint>

namespace be {
namespace util {

uint64_t hexToUInt64(const std::string& hex);
uint64_t hexToUInt64(const char* hex, int max_digits = 16);
uint64_t hexToUInt64(const char** hex, int max_digits = 16);

int64_t hexToInt64(const std::string& hex);
int64_t hexToInt64(const char* hex, int max_digits = 16);
int64_t hexToInt64(const char** hex, int max_digits = 16);

} // namespace be::util
} // namespace be

#endif
