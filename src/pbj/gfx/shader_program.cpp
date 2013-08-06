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
/// \brief  Constructs a shader program using a single vertex shader and a
///         single fragment shader.
///
/// \param  id Describes where this shader is stored in the databse.
/// \param  vertex_shader The vertex shader for this program.
/// \param  fragment_shader The fragment shader for this program.
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

/// \brief  destroys this shader program, deleting it from the GPU's memory.
ShaderProgram::~ShaderProgram()
{
    invalidate_();
}

/// \brief  Retrieves a handle to this shader program.
///
/// \return A Handle<ShaderProgram>
const be::Handle<ShaderProgram>& ShaderProgram::getHandle()
{
    return handle_;
}

/// \brief  Retrieves a handle to this shader program.
///
/// \return A ConstHandle<ShaderProgram>
const be::ConstHandle<ShaderProgram>& ShaderProgram::getHandle() const
{
    return handle_;
}

/// \brief  Retrieves the shader program's resourceId, specifying where it is
///         stored in the database.
///
/// \return The ResourceId for this shader program.

const sw::ResourceId& ShaderProgram::getId() const
{
    return resource_id_;
}

/// \brief  Retrieves the OpenGL shader program ID.
///
/// \return The OpenGL shader program ID.

GLuint ShaderProgram::getGlId() const
{
    return gl_id_;
}

/// \brief  Checks to make sure that the shader program has been linked
///         successfully.
///
/// \details If there was a problem, the info log is retrieved, a warning
///         is output, and the program is invalidated.
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

/// \brief  Invalidates this shader program, deleting it from the GPU's memory.
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
