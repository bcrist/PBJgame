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

    vec2 texture_offset;
    vec2 texture_dimensions;

    vec2 character_offset;
    F32 character_advance;

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
