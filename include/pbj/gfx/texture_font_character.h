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
/// \file   pbj/gfx/texture_font_character.h
/// \author Benjamin Crist
///
/// \brief  pbj::gfx::TextureFontCharacter class header.

#ifndef PBJ_GFX_TEXTURE_FONT_CHARACTER_H_
#define PBJ_GFX_TEXTURE_FONT_CHARACTER_H_

#include "pbj/_pbj.h"
#include "pbj/_math.h"

namespace pbj {
namespace gfx {

struct TextureFontCharacter
{
    U32 codepoint;

    vec2 tex_offset;
    vec2 tex_delta;

    vec2 dest_offset;
    F32 advance;

    static const U32 cp_invalid = 0xFFFFFFFF;

    TextureFontCharacter();

    bool operator==(const TextureFontCharacter& other) const;
    bool operator!=(const TextureFontCharacter& other) const;
    bool operator<(const TextureFontCharacter& other) const;
    bool operator<=(const TextureFontCharacter& other) const;
    bool operator>(const TextureFontCharacter& other) const;
    bool operator>=(const TextureFontCharacter& other) const;
};

} // namespace pbj::gfx
} // namespace pbj

#endif
