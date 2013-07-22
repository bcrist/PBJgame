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
template <typename Iterator>
TextureFont::TextureFont(const sw::ResourceId& id,
                         const be::ConstHandle& texture,
                         U16 line_height, U16 baseline,
                         const Iterator& chars_begin,
                         const Iterator& chars_end)
    : resource_id_(id),
      texture_(texture),
      line_height_(line_height),
      baseline_(baseline)
{
    static_assert(std::is_same<std::iterator_traits<Iterator>::value_type, TextureFontCharacter>::value,
        "chars_begin and chars_end must be iterators over TextureFontCharacter objects.");

    for (Iterator i = chars_begin; i != chars_end; ++i)
    {
        U32 codepoint = i->codepoint;

        if (codepoint == TextureFontCharacter::cp_invalid)
            default_char = *i;
        else if (codepoint < base_chars_size_)
            base_chars_[codepoint] = *i;
        else 
        {
            auto j = std::find(ext_chars_.begin(), ext_chars_.end(),
                [] (const TextureFont& a, const TextureFont& b)
                {
                    return a.codepoint == b.codepoint;
                });

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
}

} // namespace pbj::gfx
} // namespace pbj

#endif
