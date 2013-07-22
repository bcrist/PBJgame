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
/// \file   pbj/gfx/texture_font_text.h
/// \author Benjamin Crist
///
/// \brief  pbj::gfx::TextureFontText class header.

#ifndef PBJ_GFX_TEXTURE_FONT_TEXT_H_
#define PBJ_GFX_TEXTURE_FONT_TEXT_H_

#include "pbj/gfx/texture_font.h"

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
/// \brief  A mesh-like object which represents a specific text string rendered
///         using a particular TextureFont.
class TextureFontText
{
public:
    TextureFontText(const TextureFont& font, const std::string& text, GLenum buffer_mode = GL_STATIC_DRAW);
    ~TextureFontText();

    void setColor(const vec4& color);
    const vec4& getColor() const;

    void draw(const mat4& transform);

private:
    vec4 color_;
   
    be::ConstHandle<Texture> texture_;

    GLuint vao_id_; ///< OpenGL Vertex array object id
    GLuint ibo_id_; ///< OpenGL Vertex Index buffer object id
    GLuint vbo_id_; ///< OpenGL Vertex buffer object id
    GLsizei ibo_size_; ///< Size of IBO

    GLuint shader_program_id_;
    GLint color_uniform_location_;
    GLint texture_uniform_location_;
    GLint transform_uniform_location_;

    TextureFontText(const TextureFontText&);
    void operator=(const TextureFontText&);
};

} // namespace pbj::gfx
} // namespace pbj

#endif
