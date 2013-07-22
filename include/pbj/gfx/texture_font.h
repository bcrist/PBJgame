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
/// \file   pbj/gfx/texture_font.h
/// \author Benjamin Crist
///
/// \brief  pbj::gfx::TextureFont class header.

#ifndef PBJ_GFX_TEXTURE_FONT_H_
#define PBJ_GFX_TEXTURE_FONT_H_

#include "pbj/gfx/texture.h"
#include "pbj/gfx/texture_font_character.h"

namespace pbj {
namespace gfx {

class TextureFontText;

class TextureFont
{
    friend class TextureFontText;

public:
    template <typename Iterator>
    TextureFont(const sw::ResourceId& id, const be::ConstHandle& texture, ivec2 texture_size, line_height, U16 baseline, const Iterator& chars_begin, const Iterator& chars_end);
    ~TextureFont();

    const be::Handle<TextureFont>& getHandle();
    const be::ConstHandle<TextureFont>& getHandle() const;

    const sw::ResourceId& getId() const;

    const ivec2& getTextureSize() const;

    const TextureFontCharacter& operator[](U32 codepoint) const;

    F32 getTextWidth(const std::string& text);
    F32 print(const mat4& transform, const std::string& text, const vec4& color);

private:
    be::SourceHandle<TextureFont> handle_;
    sw::ResourceId resource_id_;

    sw::ResourceId texture_id_;
    be::ConstHandle<Texture> texture_;

    ivec2 texture_size_;
    
    U16 line_height_;
	unsigned short baseline_;

    static const size_t base_chars_size_ = 128;
    TextureFontCharacter default_char_;
    TextureFontCharacter base_chars_[base_chars_size_];
	std::vector<TextureFontCharacter> ext_chars_;
};

} // namespace pbj::gfx
} // namespace pbj

#include "pbj/gfx/texture_font.inl"

#endif
