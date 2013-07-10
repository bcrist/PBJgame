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
/// \file   be/util/nconvert.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of functions in "be/util/nconvert.h"

#include "be/util/nconvert.h"

#include <cassert>

namespace be {
namespace util {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Converts a hexadecimal string to an unsigned 64-bit integer.
///
/// \details A maximum of 16 hex digits are extracted.
///
/// \param  hex The hexadecimal string to convert.
/// \return The unsigned number extracted from the string.
///
/// \sa     hexToUInt64(const char**, int)
uint64_t hexToUInt64(const std::string& hex)
{
   return hexToUInt64(hex.c_str());
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Converts a hexadecimal c-string to an unsigned 64-bit integer.
///
/// \param  hex The hexadecimal c-string to convert.
/// \param  max_digits The maximum number of hex digits to extract.
/// \return The unsigned number extracted from the string.
///
/// \sa     hexToUInt64(const char**, int)
uint64_t hexToUInt64(const char* hex, int max_digits)
{
   return hexToUInt64(&hex, max_digits);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Converts a hexadecimal c-string to an unsigned 64-bit integer.
///
/// \details The const char* pointer that hex points will be updated to point
///         to the character immediately after the last character which was
///         looked at, allowing the tokenization of hex strings longer than 16
///         digits into 64-bit chunks.
///
///         Extraction continues until 'max_digits' have been extracted or the
///         end of the string is reached, whichever is first.  Any character
///         not matching the regular expression class <tt>[0-9A-Fa-f]</tt> will
///         be skipped and will not count towards the maximum digit count.
///
/// \param  hex A pointer to the the hexadecimal c-string to convert.
/// \param  max_digits The maximum number of hex digits to extract.
/// \return The unsigned number extracted from the string.
uint64_t hexToUInt64(const char** hex, int max_digits)
{
   assert(max_digits <= 16);

   uint64_t accumulator(0ULL);
   for (char c; (c = **hex) != 0 && max_digits > 0; ++*hex)
   {
      if (c >= 'a' && c <= 'f')
         c -= 'a' - 'A';

      char digit;
      switch (c)
      {
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9':
            digit = c - '0';
            break;

         case 'A':
         case 'B':
         case 'C':
         case 'D':
         case 'E':
         case 'F':
            digit = c - 'A' + 10;
            break;

         case 'a':
         case 'b':
         case 'c':
         case 'd':
         case 'e':
         case 'f':
            digit = c - 'a' + 10;
            break;

         default:
            continue;
      }

      accumulator <<= 4;
      accumulator |= digit;
      --max_digits;
   }

   return accumulator;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Converts a hexadecimal string to a 64-bit integer.
///
/// \details A maximum of 16 hex digits are extracted.
///
/// \param  hex The hexadecimal string to convert.
/// \return The number extracted from the string.
///
/// \sa     hexToInt64(const char**, int)
int64_t hexToInt64(const std::string& hex)
{
   return static_cast<int64_t>(hexToUInt64(hex));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Converts a hexadecimal c-string to a 64-bit integer.
///
/// \param  hex The hexadecimal string to convert.
/// \param  max_digits The maximum number of hex digits to extract.
/// \return The number extracted from the string.
///
/// \sa     hexToInt64(const char**, int)
int64_t hexToInt64(const char* hex, int max_digits)
{
   return static_cast<int64_t>(hexToUInt64(hex, max_digits));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Converts a hexadecimal c-string to a 64-bit integer.
///
/// \details The const char* pointer that hex points will be updated to point
///         to the character immediately after the last character which was
///         looked at, allowing the tokenization of hex strings longer than 16
///         digits into 64-bit chunks.
///
///         Extraction continues until 'max_digits' have been extracted or the
///         end of the string is reached, whichever is first.  Any character
///         not matching the regular expression class <tt>[0-9A-Fa-f]</tt> will
///         be skipped and will not count towards the maximum digit count.
///
/// \param  hex The hexadecimal string to convert.
/// \param  max_digits The maximum number of hex digits to extract.
/// \return The number extracted from the string.
int64_t hexToInt64(const char** hex, int max_digits)
{
   return static_cast<int64_t>(hexToUInt64(hex, max_digits));
}

} // namespace be::util
} // namespace be
