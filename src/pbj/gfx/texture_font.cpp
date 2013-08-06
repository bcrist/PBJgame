///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/texture_font.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::gfx::TextureFont functions.

#include "pbj/gfx/texture_font.h"

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
TextureFont::~TextureFont()
{
}

///////////////////////////////////////////////////////////////////////////////
const be::Handle<TextureFont>& TextureFont::getHandle()
{
    return handle_;
}

///////////////////////////////////////////////////////////////////////////////
const be::ConstHandle<TextureFont>& TextureFont::getHandle() const
{
    return handle_;
}

///////////////////////////////////////////////////////////////////////////////
const sw::ResourceId& TextureFont::getId() const
{
    return resource_id_;
}

///////////////////////////////////////////////////////////////////////////////
F32 TextureFont::getCapHeight() const
{
    return cap_height_;
}

///////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////
F32 TextureFont::getTextWidth(const std::string& text)
{
    // TODO: implement if necessary.
    throw std::exception("Not yet implemented!");
}

///////////////////////////////////////////////////////////////////////////////
F32 TextureFont::print(const mat4& transform, const std::string& text, const vec4& color)
{
    // use TextureFontText instead 

    // TODO: implement if necessary.
    throw std::exception("Not yet implemented!");
}


} // namespace pbj::gfx
} // namespace pbj
