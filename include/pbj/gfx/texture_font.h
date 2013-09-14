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

///////////////////////////////////////////////////////////////////////////////
/// \brief  Allows the drawing of text in OpenGL through the use of a font
///         sprite sheet.
class TextureFont
{
public:
    TextureFont(const be::ConstHandle<Texture>& texture, F32 cap_height, const std::vector<TextureFontCharacter>& characters);
    ~TextureFont();

    F32 getCapHeight() const;

    const TextureFontCharacter& operator[](U32 codepoint) const;

    void print(const std::string& text) const;
    F32 getTextWidth(const std::string& text) const;

private:
    Texture* texture_;

    F32 cap_height_;

    static const size_t base_chars_size_ = 128;
    TextureFontCharacter default_char_;
    TextureFontCharacter base_chars_[base_chars_size_];
	std::vector<TextureFontCharacter> ext_chars_;
};

} // namespace pbj::gfx
} // namespace pbj

#include "pbj/gfx/texture_font.inl"

#endif
