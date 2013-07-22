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
/// \file   pbj/gfx/texture_font_text.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::gfx::TextureFontText functions.

#include "pbj/gfx/texture_font_text.h"

#include "pbj/engine.h"
#include "pbj/gfx/shader_program.h"

namespace pbj {
namespace gfx {

TextureFontText::TextureFontText(const TextureFont& font, const std::string& text, GLenum buffer_mode)
    : color_(1.0f, 1.0f, 1.0f, 1.0f)
{
    // calculate vertex data
    std::vector<U16> vert_indices;
    std::vector<vec2> verts;
    vert_indices.reserve(text.length() * 6);    // 6 vertex indices (2 tris) per character
    verts.reserve(text.length() * 8);  // 4 vertices & 4 texcoords per character

    vec2 cursor; // where the current character should be drawn

    F32 scale_x = 1.0f / font.getTextureSize().x;
    F32 scale_y = 1.0f / font.getTextureSize().y;

    for (char c : text)
    {
        const TextureFontCharacter& ch = font[c];

        // indices
        U16 base_index = verts.size() >> 1;     // 1 vertex & 1 texcoord per index

        vert_indices.push_back(base_index);     // first tri
        vert_indices.push_back(base_index + 1);
        vert_indices.push_back(base_index + 2);

        vert_indices.push_back(base_index);     // second tri
        vert_indices.push_back(base_index + 2);
        vert_indices.push_back(base_index + 3);

        // vertices
        vec2 bottom_left(cursor + ch.dest_offset);
        vec2 top_right(bottom_left + ch.tex_delta);
        vec2 tex_bottom_left(ch.tex_offset);
        vec2 tex_top_right(ch.tex_offset + ch.tex_delta);
        tex_bottom_left.x *= scale_x; tex_top_right.x *= scale_x;
        tex_bottom_left.y *= scale_y; tex_top_right.y *= scale_y;

        verts.push_back(bottom_left);       // vertex
        verts.push_back(tex_bottom_left);   // texcoord

        verts.push_back(vec2(top_right.x, bottom_left.y));
        verts.push_back(vec2(tex_top_right.x, tex_bottom_left.y));

        verts.push_back(top_right);
        verts.push_back(tex_top_right);

        verts.push_back(vec2(bottom_left.x, top_right.y));
        verts.push_back(vec2(tex_bottom_left.x, tex_top_right.y));
        
        cursor.x += ch.advance;
    }

    texture_ = font.texture_;

    glGenVertexArrays(1, &vao_id_);
    glBindVertexArray(vao_id_);

    glGenBuffers(1, &ibo_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vert_indices.size() * sizeof(U16), vert_indices.data(), buffer_mode);
    ibo_size_ = vert_indices.size();

    glGenBuffers(1, &vbo_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(vec2), verts.data(), buffer_mode);
    // vertex positions at shader index 0:
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(vec2), 0);
    // texture coords at shader index 1:
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(vec2), reinterpret_cast<void*>(sizeof(vec2)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);   // unbind VBO because GL_ARRAY_BUFFER is not part of the VAO state.
    glBindVertexArray(0);               // unbind VAO


    Engine& engine = getEngine();
    const BuiltIns& built_ins = engine.getBuiltIns();

    shader_program_id_ = built_ins.getProgram(Id("ShaderProgram.TextureFontText")).getGlId();
    color_uniform_location_ = glGetUniformLocation(shader_program_id_, "color");
    texture_uniform_location_ = glGetUniformLocation(shader_program_id_, "texture");
    transform_uniform_location_ = glGetUniformLocation(shader_program_id_, "transform");
}

TextureFontText::~TextureFontText()
{
    if (vao_id_ != 0)
        glDeleteVertexArrays(1, &vao_id_);

    if (ibo_id_ != 0)
        glDeleteBuffers(1, &ibo_id_);

    if (vbo_id_ != 0)
        glDeleteBuffers(1, &vbo_id_);
}

void TextureFontText::setColor(const vec4& color)
{
    color_ = color;
}

const vec4& TextureFontText::getColor() const
{
    return color_;
}

void TextureFontText::draw(const mat4& transform)
{
    if (vao_id_ == 0)
    {
        // TODO: log warning
        return;
    }

    glUseProgram(shader_program_id_); // bind program
    glUniform1i(texture_uniform_location_, 0);
    glActiveTexture(GL_TEXTURE0);

    const Texture* tex = texture_.get();
    glBindTexture(GL_TEXTURE_2D, tex ? tex->getGlId() : 0);

    glBindVertexArray(vao_id_); // bind VAO

    glUniform4fv(color_uniform_location_, 1, glm::value_ptr(color_));
    glUniformMatrix4fv(transform_uniform_location_, 1, false, glm::value_ptr(transform));

    // draw text
    glDrawElements(GL_TRIANGLES, ibo_size_, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0); // unbind VAO

    glUseProgram(0); // unbind program
}
    
} // namespace pbj::gfx
} // namespace pbj
