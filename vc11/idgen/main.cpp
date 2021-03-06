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
/// \file   main.cpp
/// \author Benjamin Crist
///
/// \brief  Command-line utility for calculating be::Id values from strings

#include <iostream>
#include <string>

#include "be/id.h"

///////////////////////////////////////////////////////////////////////////////
/// \brief  Utility for generating FNV-1a hashes of arbitrary strings.
///
/// \details If there are any command-line arguments, each argument is
///         processed as a string to be hashed.  If there are no command-line
///         arguments, STDIN will be read line-by-line until all input is
///         exhausted.
///
///         Each string input will output a single line to STDOUT with the hash
///         value of the text entered.  The format is identical to that used
///         by Id::to_string() with Id names enabled:
///
/// \code
///         #0000000000000000:Original Text Here
/// \endcode
///
///         When a blank line is encountered when processing from STDIN, a
///         blank line is output to STDOUT (rather than hashing the empty
///         string and outputting that).
///
///         If a hash collision occurs (the hash generated is the same as a
///         different, previously processed input string) a warning will be
///         output to STDERR.
///
/// \param  argc The number of command-line arguments (including the program
///         name).
/// \param  argv An array of c-strings representing the command-line arguments
///
/// \return Always returns 0 to the operating system.
int main(int argc, char** argv)
{
   if (argc > 1)
   {
      for (int i = 1; i < argc; ++i)
      {
         std::cout << be::Id(argv[i]) << std::endl;
      }
   }
   else
   {
      while (std::cin)
      {
         std::string str;
         std::getline(std::cin, str);

         if (str.length() > 0)
            std::cout << be::Id(str) << std::endl;
         else
            std::cout << std::endl;
      }
   }

   return 0;
}
