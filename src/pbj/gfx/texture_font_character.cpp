///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/texture_font_character.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::gfx::TextureFontCharacter functions.

#include "pbj/gfx/texture_font_character.h"

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
TextureFontCharacter::TextureFontCharacter()
    : codepoint(cp_invalid),
      character_advance(0)
{
}

///////////////////////////////////////////////////////////////////////////////
bool TextureFontCharacter::operator==(const TextureFontCharacter& other) const
{
    return codepoint == other.codepoint;
}

///////////////////////////////////////////////////////////////////////////////
bool TextureFontCharacter::operator!=(const TextureFontCharacter& other) const
{
    return codepoint != other.codepoint;
}

///////////////////////////////////////////////////////////////////////////////
bool TextureFontCharacter::operator<(const TextureFontCharacter& other) const
{
    return codepoint < other.codepoint;
}

///////////////////////////////////////////////////////////////////////////////
bool TextureFontCharacter::operator<=(const TextureFontCharacter& other) const
{
    return codepoint <= other.codepoint;
}

///////////////////////////////////////////////////////////////////////////////
bool TextureFontCharacter::operator>(const TextureFontCharacter& other) const
{
    return codepoint > other.codepoint;
}

///////////////////////////////////////////////////////////////////////////////
bool TextureFontCharacter::operator>=(const TextureFontCharacter& other) const
{
    return codepoint >= other.codepoint;
}

} // namespace pbj::gfx
} // namespace pbj
