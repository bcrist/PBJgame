///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/texture_font_character.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::gfx::TextureFontCharacter functions.

#include "pbj/gfx/texture_font_character.h"

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new invalid TextureFontCharacter
TextureFontCharacter::TextureFontCharacter()
    : codepoint(cp_invalid),
      character_advance(0)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares two TextureFontCharacters to see if they represent the
///         same codepoint.
///
/// \details It is assumed that the objects belong to the same TextureFont.
///
/// \param  other The right hand side of the == operation.
/// \return true if the characters represent the same codepoint.
bool TextureFontCharacter::operator==(const TextureFontCharacter& other) const
{
    return codepoint == other.codepoint;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares two TextureFontCharacters to see if they represent
///         different codepoints.
///
/// \details It is assumed that the objects belong to the same TextureFont.
///
/// \param  other The right hand side of the != operation.
/// \return true if the characters do not represent the same codepoint.
bool TextureFontCharacter::operator!=(const TextureFontCharacter& other) const
{
    return codepoint != other.codepoint;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares two TextureFontCharacters to see if this one has a smaller
///         codepoint than the right hand side.
///
/// \details It is assumed that the objects belong to the same TextureFont.
///
/// \param  other The right hand side of the < operation.
/// \return true if this character represents a smaller codepoint than the
///         other character.
bool TextureFontCharacter::operator<(const TextureFontCharacter& other) const
{
    return codepoint < other.codepoint;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares two TextureFontCharacters to see if this one has a smaller
///         or equal codepoint than the right hand side.
///
/// \details It is assumed that the objects belong to the same TextureFont.
///
/// \param  other The right hand side of the <= operation.
/// \return true if this character represents a not larger codepoint than the
///         other character.
bool TextureFontCharacter::operator<=(const TextureFontCharacter& other) const
{
    return codepoint <= other.codepoint;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares two TextureFontCharacters to see if this one has a bigger
///         codepoint than the right hand side.
///
/// \details It is assumed that the objects belong to the same TextureFont.
///
/// \param  other The right hand side of the > operation.
/// \return true if this character represents a bigger codepoint than the
///         other character.
bool TextureFontCharacter::operator>(const TextureFontCharacter& other) const
{
    return codepoint > other.codepoint;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compares two TextureFontCharacters to see if this one has a bigger
///         or equal codepoint than the right hand side.
///
/// \details It is assumed that the objects belong to the same TextureFont.
///
/// \param  other The right hand side of the >= operation.
/// \return true if this character represents a not smaller codepoint than the
///         other character.
bool TextureFontCharacter::operator>=(const TextureFontCharacter& other) const
{
    return codepoint >= other.codepoint;
}

} // namespace pbj::gfx
} // namespace pbj
