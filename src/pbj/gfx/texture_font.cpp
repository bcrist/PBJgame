///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/texture_font.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::gfx::TextureFont functions.

#include "pbj/gfx/texture_font.h"

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destroys the TextureFont.
TextureFont::~TextureFont()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a handle to this TextureFont.
///
/// \return A Handle<TextureFont> referring to this object.
const be::Handle<TextureFont>& TextureFont::getHandle()
{
    return handle_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a handle to this TextureFont.
///
/// \return A ConstHandle<TextureFont> referring to this object.
const be::ConstHandle<TextureFont>& TextureFont::getHandle() const
{
    return handle_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves this TextureFont's ResourceId.
///
/// \return A ResourceId defining the location of the font in a database.
const sw::ResourceId& TextureFont::getId() const
{
    return resource_id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the cap height specified for this font.
///
/// \return The cap height.
F32 TextureFont::getCapHeight() const
{
    return cap_height_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a TextureFontCharacter object corresponding to a
///         particular unicode codepoint.
///
/// \details If the codepoint does not have a TextureFontCharacter defined
///         for it, a placeholder character is used.
///
/// \param  codepoint The unicode codepoint to retrieve a TextureFontCharacter
///         for.
/// \return A TextureFontCharacter for drawing the codepoint requested.
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

} // namespace pbj::gfx
} // namespace pbj
