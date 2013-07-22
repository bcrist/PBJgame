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
/// \file   pbj/gfx/shader_program.h
/// \author Benjamin Crist
///
/// \brief  pbj::gfx::ShaderProgram class header.

#ifndef PBJ_GFX_SHADER_PROGRAM_H_
#define PBJ_GFX_SHADER_PROGRAM_H_

#include "pbj/gfx/shader.h"

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Represents a program consisting of one or more shaders.
///
/// \details Usually a single vertex shader and a single fragment shader are
///         used together.
///
/// \sa     Shader
class ShaderProgram
{
public:
    ShaderProgram(const sw::ResourceId& id, const Shader& vertex_shader, const Shader& fragment_shader);
    template <typename Iterator>
    ShaderProgram(const sw::ResourceId& id, const Iterator& begin, const Iterator& end);
    ~ShaderProgram();

    const be::Handle<ShaderProgram>& getHandle();
    const be::ConstHandle<ShaderProgram>& getHandle() const;

    const sw::ResourceId& getId() const;

    GLuint getGlId() const;

private:
    void checkLinkResult_();
    void invalidate_();

    be::SourceHandle<ShaderProgram> handle_;
    sw::ResourceId resource_id_;

    GLuint gl_id_;

    ShaderProgram(const ShaderProgram&);
    void operator=(const ShaderProgram&);
};

} // namespace pbj::gfx
} // namespace pbj

#include "pbj/gfx/shader_program.inl"

#endif
