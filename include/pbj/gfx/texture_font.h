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
    TextureFont(const sw::ResourceId& id, const be::ConstHandle<Texture>& texture, F32 cap_height, const Iterator& chars_begin, const Iterator& chars_end);
    ~TextureFont();

    const be::Handle<TextureFont>& getHandle();
    const be::ConstHandle<TextureFont>& getHandle() const;

    const sw::ResourceId& getId() const;

    F32 getCapHeight() const;

    const TextureFontCharacter& operator[](U32 codepoint) const;

    F32 getTextWidth(const std::string& text);
    F32 print(const mat4& transform, const std::string& text, const vec4& color);

private:
    be::SourceHandle<TextureFont> handle_;
    sw::ResourceId resource_id_;

    sw::ResourceId texture_id_;
    be::ConstHandle<Texture> texture_;

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
