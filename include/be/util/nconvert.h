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
