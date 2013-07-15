// Copyright (c) 2013 PBJ Productions
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
/// \file   pbj/scene/shader.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::scene::Shader functions

#include "pbj/scene/shader.h"

#include <cassert>
#include <iostream>

namespace pbj {
namespace scene {

Shader::Shader(const Id& id, Type type, const std::string& source)
    : type_(type),
      id_(id),
      gl_id_(0)
{
    handle_.associate(this);

    compile_(source);
}

Shader::~Shader()
{
    invalidate_();
}

be::Handle<Shader> Shader::getHandle()
{
    return handle_;
}

const be::ConstHandle<Shader> Shader::getHandle() const
{
    return handle_;
}

Shader::Type Shader::getType() const
{
    return type_;
}

GLuint Shader::getGlId() const
{
    return gl_id_;
}

#ifdef PBJ_EDITOR
Shader::Shader()
    : type_(TFragment),
      gl_id_(0)
{
    handle_.associate(this);
}

void Shader::setName(const std::string& name)
{
    name_ = name;
    id_ = Id(name);
}

const std::string& Shader::getName() const
{
    return name_;
}

void Shader::setSource(const std::string& source)
{
    source_ = source;
    info_log_.clear();
    invalidate_();
}

const std::string& Shader::getSource() const
{
    return source_;
}

void Shader::setType(Type type)
{
    if (type != type_)
    {
        type_ = type;
        invalidate_();
    }
}

bool Shader::isValid() const
{
    return gl_id_ != 0;
}

const std::string& Shader::getInfoLog() const
{
    return info_log_;
}

void Shader::compile()
{
    compile_(source_);
}

#endif

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

    GLint result = GL_FALSE;
    glGetShaderiv(gl_id_, GL_COMPILE_STATUS, &result);

    if (result != GL_TRUE)
    {
        GLint infolog_len;
        glGetShaderiv(gl_id_, GL_INFO_LOG_LENGTH, &infolog_len);
        char *infolog = new char[std::max(1, infolog_len)];
        glGetShaderInfoLog(gl_id_, infolog_len, NULL, infolog);

        PBJ_LOG(VError) << "Error compiling shader!" << PBJ_LOG_NL
                        << "        Shader ID: " << id_ << PBJ_LOG_NL
                        << "      Shader Type: " << (type_ == TVertex ? "Vertex" : (type_ == TFragment ? "Fragment" : "Unknown")) << PBJ_LOG_NL
                        << "   GL Shader Type: " << gl_type << PBJ_LOG_NL
                        << "GL Compile Status: " << result << PBJ_LOG_NL
                        << "      GL Info Log: " << infolog << PBJ_LOG_NL
                        << "    Shader Source: " << source << PBJ_LOG_END;

#ifdef PBJ_EDITOR
        info_log_ = std::string(infolog);
#endif
        delete[] infolog;

        invalidate_();
        throw std::runtime_error("Error compiling shader!");
    }
#ifdef PBJ_EDITOR
    else
    {
        GLint infolog_len;
        glGetShaderiv(gl_id_, GL_INFO_LOG_LENGTH, &infolog_len);
        char *infolog = new char[std::max(1, infolog_len)];
        glGetShaderInfoLog(gl_id_, infolog_len, NULL, infolog);
        info_log_ = std::string(infolog);
        delete[] infolog;
    }
#endif
}

void Shader::invalidate_()
{
    if (gl_id_ != 0)
    {
        glDeleteShader(gl_id_);
        gl_id_ = 0;
    }
}

} // namespace pbj::scene
} // namespace pbj
