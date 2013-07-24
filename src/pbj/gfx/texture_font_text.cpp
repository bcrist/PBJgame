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

TextureFontText::TextureFontText()
    : color_(1.0f, 1.0f, 1.0f, 1.0f),
      buffer_mode_(GL_STATIC_DRAW),
      buffers_valid_(false)
{
    glGenVertexArrays(1, &vao_id_);
    glGenBuffers(1, &ibo_id_);
    glGenBuffers(1, &vbo_id_);
    
    Engine& engine = getEngine();
    const BuiltIns& built_ins = engine.getBuiltIns();

    shader_program_id_ = built_ins.getProgram(Id("ShaderProgram.TextureFontText")).getGlId();
    color_uniform_location_ = glGetUniformLocation(shader_program_id_, "color");
    texture_uniform_location_ = glGetUniformLocation(shader_program_id_, "texsampler");
    transform_uniform_location_ = glGetUniformLocation(shader_program_id_, "transform");
}

TextureFontText::~TextureFontText()
{
    glDeleteVertexArrays(1, &vao_id_);
    glDeleteBuffers(1, &ibo_id_);
    glDeleteBuffers(1, &vbo_id_);
}

void TextureFontText::setBufferMode(GLenum buffer_mode)
{
    if (buffer_mode_ != buffer_mode)
    {
        buffer_mode_ = buffer_mode;
        buffers_valid_ = false;
    }
}

GLenum TextureFontText::getBufferMode() const
{
    return buffer_mode_;
}

void TextureFontText::setColor(const vec4& color)
{
    color_ = color;
}

const vec4& TextureFontText::getColor() const
{
    return color_;
}

void TextureFontText::setFont(const be::ConstHandle<TextureFont>& font)
{
    if (font != font_)
    {
        font_ = font;
        const TextureFont* f = font.get();
        texture_ = (f ? f->texture_ : be::ConstHandle<Texture>());

        buffers_valid_ = false;
    }
}

const be::ConstHandle<TextureFont>& TextureFontText::getFont() const
{
    return font_;
}

void TextureFontText::setText(const std::string& text)
{
    text_ = text;
    buffers_valid_ = false;
}

const std::string& TextureFontText::getText() const
{
    return text_;
}

F32 TextureFontText::getTextWidth()
{
    if (!isPrepared())
        prepare();

    return text_width_;
}

void TextureFontText::prepare()
{
    bool font_data_valid = true;

    const Texture* tex = texture_.get();
    if (!tex)
    {
        // TODO: log warning
        font_data_valid = false;
    }

    const TextureFont* font = font_.get();
    if (!font)
    {
        // TODO: log warning
        font_data_valid = false;
    }

    // calculate vertex data
    std::vector<U16> vert_indices;
    std::vector<vec2> verts;
    vec2 cursor; // where the current character should be drawn

    if (font_data_valid)
    {
        vert_indices.reserve(text_.length() * 6);    // 6 vertex indices (2 tris) per character
        verts.reserve(text_.length() * 8);  // 4 vertices & 4 texcoords per character

        F32 scale_x = 1.0f / tex->getDimensions().x;
        F32 scale_y = 1.0f / tex->getDimensions().y;

        for (char c : text_)
        {
            const TextureFontCharacter& ch = (*font)[c];

            // indices
            U16 base_index = verts.size() >> 1;     // 1 vertex & 1 texcoord per index

            vert_indices.push_back(base_index);     // first tri
            vert_indices.push_back(base_index + 1);
            vert_indices.push_back(base_index + 2);

            vert_indices.push_back(base_index);     // second tri
            vert_indices.push_back(base_index + 2);
            vert_indices.push_back(base_index + 3);

            // vertices
            vec2 top_left(cursor + ch.character_offset);
            vec2 bottom_right(top_left.x + ch.texture_dimensions.x, top_left.y - ch.texture_dimensions.y);
            vec2 tex_top_left(ch.texture_offset);
            vec2 tex_bottom_right(tex_top_left + ch.texture_dimensions);
            tex_top_left.x *= scale_x; tex_bottom_right.x *= scale_x;
            tex_top_left.y *= scale_y; tex_bottom_right.y *= scale_y;

            verts.push_back(top_left);      // vertex pos
            verts.push_back(tex_top_left);  // tex coord

            verts.push_back(vec2(top_left.x, bottom_right.y));
            verts.push_back(vec2(tex_top_left.x, tex_bottom_right.y));

            verts.push_back(bottom_right);
            verts.push_back(tex_bottom_right);

            verts.push_back(vec2(bottom_right.x, top_left.y));
            verts.push_back(vec2(tex_bottom_right.x, tex_top_left.y));
            
            cursor.x += ch.character_advance;
        }
    }

    text_width_ = cursor.x;

    ibo_size_ = vert_indices.size();

    glBindVertexArray(vao_id_); // bind VAO

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id_); // set VAO's bound IBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_size_ * sizeof(U16), vert_indices.data(), buffer_mode_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);  // bind VBO
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(vec2), verts.data(), buffer_mode_);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(vec2), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(vec2), reinterpret_cast<void*>(sizeof(vec2)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);       // unbind VBO

    glEnableVertexAttribArray(0); // vertex positions at shader index 0
    glEnableVertexAttribArray(1); // texture coords at shader index 1

    glBindVertexArray(0);   // unbind VAO
}

bool TextureFontText::isPrepared() const
{
    return buffers_valid_;
}

void TextureFontText::draw(const mat4& transform)
{
    if (!isPrepared())
        prepare();

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
