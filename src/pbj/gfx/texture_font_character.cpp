// Copyright (c) 2013 PBJ^2 Productions
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
/// \file   pbj/gfx/texture_font_character.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::gfx::TextureFontCharacter functions.

#include "pbj/gfx/texture_font_character.h"

namespace pbj {
namespace gfx {

TextureFontCharacter::TextureFontCharacter()
    : codepoint(cp_invalid),
      advance(0)
{}

bool TextureFontCharacter::operator==(const TextureFontCharacter& other) const
{
    return codepoint == other.codepoint;
}

bool TextureFontCharacter::operator!=(const TextureFontCharacter& other) const
{
    return codepoint != other.codepoint;
}

bool TextureFontCharacter::operator<(const TextureFontCharacter& other) const
{
    return codepoint < other.codepoint;
}

bool TextureFontCharacter::operator<=(const TextureFontCharacter& other) const
{
    return codepoint <= other.codepoint;
}

bool TextureFontCharacter::operator>(const TextureFontCharacter& other) const
{
    return codepoint > other.codepoint;
}

bool TextureFontCharacter::operator>=(const TextureFontCharacter& other) const
{
    return codepoint >= other.codepoint;
}

} // namespace pbj::gfx
} // namespace pbj
