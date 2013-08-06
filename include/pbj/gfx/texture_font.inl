///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/texture_font.inl
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::gfx::TextureFont template functions.

#if !defined(PBJ_GFX_TEXTURE_FONT_H_) && !defined(DOXYGEN)
#include "pbj/gfx/texture_font.h"
#elif !defined(PBJ_GFX_TEXTURE_FONT_INL_)
#define PBJ_GFX_TEXTURE_FONT_INL_

#include <algorithm>

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a texture font from a texture and a series of
///         TextureFontCharacters describing the location of characters in the
///         texture.
///
/// \param  id Describes the location of the texture font in the database it
///         was loaded from.
/// \param  texture A ConstHandle to the texture for drawing the font.
/// \param  cap_height Not required to actually render text, but describes
///         the height of a typical latin uppercase character.
/// \param  chars_begin An iterator to the first TextureFontCharacter in
///         this font.
/// \param  chars_end An iterator to the point just after the last
///         TextureFontCharacter in this font.
template <typename Iterator>
TextureFont::TextureFont(const sw::ResourceId& id,
                         const be::ConstHandle<Texture>& texture,
                         F32 cap_height,
                         const Iterator& chars_begin,
                         const Iterator& chars_end)
    : resource_id_(id),
      cap_height_(cap_height),
      texture_(texture)
{
    static_assert(std::is_same<std::iterator_traits<Iterator>::value_type, TextureFontCharacter>::value,
        "chars_begin and chars_end must be iterators over TextureFontCharacter objects.");

    handle_.associate(this);

    for (Iterator i = chars_begin; i != chars_end; ++i)
    {
        U32 codepoint = i->codepoint;

        if (codepoint == TextureFontCharacter::cp_invalid)
            default_char_ = *i;
        else if (codepoint < base_chars_size_)
            base_chars_[codepoint] = *i;
        else 
        {
            TextureFontCharacter query;
            query.codepoint = codepoint;
            auto j = std::find(ext_chars_.begin(), ext_chars_.end(), query);

            if (j == ext_chars_.end())
            {
                // character hasn't been seen yet
                ext_chars_.push_back(*i);
            }
            else
            {
                // overwrite previous character
                *j = *i;
            }
        }
    }

    std::sort(ext_chars_.begin(), ext_chars_.end());
}

} // namespace pbj::gfx
} // namespace pbj

#endif
