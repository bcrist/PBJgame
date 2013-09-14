///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/shader.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::gfx::Shader functions

#include "pbj/gfx/shader.h"

#include <cassert>
#include <iostream>

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new shader object by uploading the shader source
///         to the GPU and compiling it.
///
/// \param  id Describes the location of the shader in the database it was
///         loaded from.
/// \param  type The type of shader: vertex or fragment.
/// \param  source A string containing the GLSL source code to compile.
Shader::Shader(const sw::ResourceId& id, Type type, const std::string& source)
    : type_(type),
      id_(id),
      gl_id_(0)
{
    compile_(source);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  destroys the shader object, ensuring that it is deleted from the
///         GPU.
Shader::~Shader()
{
    invalidate_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves this shader's ResourceId
///
/// \return A ResourceId defining the location of this shader in a database.
const sw::ResourceId& Shader::getId() const
{
    return id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the type of this shader.
///
/// \return Type::TVertex or Type::TFragment.
Shader::Type Shader::getType() const
{
    return type_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the OpenGL ID of the compiled shader object.
///
/// \return the OpenGL shader object ID.
GLuint Shader::getGlId() const
{
    return gl_id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Helper for uploading source data to the GPU and compiling it.
///
/// \param  source The shader's source code.
void Shader::compile_(const std::string& source)
{
    invalidate_();

    GLuint gl_type;
    switch (type_)
    {
        case TVertex:
            gl_type = GL_VERTEX_SHADER;
            break;

        case TFragment:
            gl_type = GL_FRAGMENT_SHADER;
            break;

        default:
            assert(false);  // We are only dealing with vertex and fragment shaders
    }

    gl_id_ = glCreateShader(gl_type);

    const char* source_cstr = source.c_str();
    glShaderSource(gl_id_, 1, &source_cstr, NULL);
    glCompileShader(gl_id_);

    checkCompileResult_(source);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Checks if there was a problem with compilation.
/// 
/// \throws std::runtime_error If the shader was not compiled successfully.
void Shader::checkCompileResult_(const std::string& source)
{
    GLint result = GL_FALSE;
    glGetShaderiv(gl_id_, GL_COMPILE_STATUS, &result);

    // if there was a problem, get the shader info log, display a warning, and throw an error.
    if (result != GL_TRUE)
    {
        GLint infolog_len;
        glGetShaderiv(gl_id_, GL_INFO_LOG_LENGTH, &infolog_len);
        char *infolog = new char[std::max(1, infolog_len)];
        glGetShaderInfoLog(gl_id_, infolog_len, NULL, infolog);

        PBJ_LOG(VError) << "Error compiling shader!" << PBJ_LOG_NL
                        << "      Sandwich ID: " << id_.sandwich << PBJ_LOG_NL
                        << "        Shader ID: " << id_.resource << PBJ_LOG_NL
                        << "      Shader Type: " << (type_ == TVertex ? "Vertex" : (type_ == TFragment ? "Fragment" : "Unknown")) << PBJ_LOG_NL
                        << "GL Compile Status: " << result << PBJ_LOG_NL
                        << "      GL Info Log: " << infolog << PBJ_LOG_NL
                        << "    Shader Source: " << source << PBJ_LOG_END;

        delete[] infolog;

        invalidate_();
        throw std::runtime_error("Error compiling shader!");
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Invalidates the shader, deleting it from GPU memory.
void Shader::invalidate_()
{
    if (gl_id_ != 0)
    {
        glDeleteShader(gl_id_);
        gl_id_ = 0;
    }
}

} // namespace pbj::gfx
} // namespace pbj
