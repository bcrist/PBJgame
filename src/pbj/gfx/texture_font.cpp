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
/// \file   pbj/gfx/texture_font.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::gfx::TextureFont functions.

#include "pbj/gfx/texture_font.h"

namespace pbj {
namespace gfx {

TextureFont::~TextureFont()
{
}

const be::Handle<TextureFont>& TextureFont::getHandle()
{
    return handle_;
}

const be::ConstHandle<TextureFont>& TextureFont::getHandle() const
{
    return handle_;
}

const sw::ResourceId& TextureFont::getId() const
{
    return resource_id_;
}

const ivec2& TextureFont::getTextureSize() const
{
    return texture_size_;
}

const TextureFontCharacter& TextureFont::operator[](U32 codepoint) const
{
    const TextureFontCharacter* ch(nullptr);
    if (codepoint < base_chars_size_)
    {
        ch = &(base_chars_[codepoint]);
    }
    else
    {
        TextureFontCharacter query;
        query.codepoint = codepoint;
        auto i = std::lower_bound(ext_chars_.begin(), ext_chars_.end(), query,
            [](const TextureFontCharacter& a, const TextureFontCharacter& b)
            {
                return a.codepoint < b.codepoint;
            });

        if (i != ext_chars_.end())
        {
            ch = &(*i);
        }
    }

    if (ch == nullptr || ch->codepoint == TextureFontCharacter::cp_invalid)
        ch = &default_char_;

    return *ch;
}

F32 TextureFont::getTextWidth(const std::string& text)
{
    // TODO: implement if necessary.
    throw std::exception("Not yet implemented!");
}

F32 TextureFont::print(const mat4& transform, const std::string& text, const vec4& color)
{
    // use TextureFontText instead 

    // TODO: implement if necessary.
    throw std::exception("Not yet implemented!");
}


} // namespace pbj::gfx
} // namespace pbj
