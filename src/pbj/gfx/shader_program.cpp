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

#include "pbj/gfx/shader_program.h"

#include <iostream>

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
ShaderProgram::ShaderProgram(const sw::ResourceId& id, const Shader& vertex_shader, const Shader& fragment_shader)
    : resource_id_(id),
      gl_id_(0)
{
#ifdef PBJ_DEBUG
    if (vertex_shader.getType() != Shader::TVertex)
        PBJ_LOG(VNotice) << "Expected first parameter to be vertex shader!" << PBJ_LOG_NL
                         << "       Sandwich ID: " << id.sandwich << PBJ_LOG_NL
                         << "        Program ID: " << id.resource << PBJ_LOG_NL
                         << "Shader Sandwich ID: " << vertex_shader.getId().sandwich << PBJ_LOG_NL
                         << "         Shader ID: " << vertex_shader.getId().resource << PBJ_LOG_END;

    if (fragment_shader.getType() != Shader::TFragment)
        PBJ_LOG(VNotice) << "Expected second parameter to be fragment shader!" << PBJ_LOG_NL
                         << "       Sandwich ID: " << id.sandwich << PBJ_LOG_NL
                         << "        Program ID: " << id.resource << PBJ_LOG_NL
                         << "Shader Sandwich ID: " << fragment_shader.getId().sandwich << PBJ_LOG_NL
                         << "         Shader ID: " << fragment_shader.getId().resource << PBJ_LOG_END;
#endif

    handle_.associate(this);

    gl_id_ = glCreateProgram();
    glAttachShader(gl_id_, vertex_shader.getGlId());
    glAttachShader(gl_id_, fragment_shader.getGlId());
    glLinkProgram(gl_id_);

    checkLinkResult_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief
/// \details
ShaderProgram::~ShaderProgram()
{
    invalidate_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief
/// \details
const be::Handle<ShaderProgram>& ShaderProgram::getHandle()
{
    return handle_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief
/// \details
const be::ConstHandle<ShaderProgram>& ShaderProgram::getHandle() const
{
    return handle_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief
/// \details
const sw::ResourceId& ShaderProgram::getId() const
{
    return resource_id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief
/// \details
GLuint ShaderProgram::getGlId() const
{
    return gl_id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief
/// \details
void ShaderProgram::checkLinkResult_()
{
    GLint result = GL_FALSE;
    glGetProgramiv(gl_id_, GL_LINK_STATUS, &result);

    if (result != GL_TRUE)
    {
        GLint infolog_len;
        glGetProgramiv(gl_id_, GL_INFO_LOG_LENGTH, &infolog_len);
        char *infolog = new char[std::max(1, infolog_len)];
        glGetProgramInfoLog(gl_id_, infolog_len, NULL, infolog);

        PBJ_LOG(VError) << "Error linking program!" << PBJ_LOG_NL
                        << "   Sandwich ID: " << resource_id_.sandwich << PBJ_LOG_NL
                        << "    Program ID: " << resource_id_.resource << PBJ_LOG_NL
                        << "GL Link Status: " << result << PBJ_LOG_NL
                        << "   GL Info Log: " << infolog << PBJ_LOG_END;

        delete[] infolog;

        invalidate_();
        throw std::runtime_error("Error linking program!");
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief
/// \details
void ShaderProgram::invalidate_()
{
    if (gl_id_ != 0)
    {
        glDeleteProgram(gl_id_);
        gl_id_ = 0;
    }
}

} // namespace pbj::gfx
} // namespace pbj
