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

Shader::Shader(const AssetId& id, Type type, const std::string& source)
    : type_(type),
      asset_id_(id),
      gl_id_(0)
{
    handle_.associate(this);

#ifdef PBJ_EDITOR
    setSource(source);
#endif

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

const AssetId& Shader::getAssetId() const
{
    return asset_id_;
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
    metadata_["__name__"] = name;
    asset_id_.asset = Id(name);
}

const std::string& Shader::getName() const
{
    return getMetadata("__name__");
}

void Shader::setBed(const Id& id)
{
    asset_id_.bed = id;
}

void Shader::setMetadata(const std::string& key, const std::string& value)
{
    if (key[0] == '_' && key[1] == '_')
    {
        PBJ_LOG(VNotice) << "Attempted to set invalid metadata property!" << PBJ_LOG_NL
                         << "        Bed ID: " << asset_id_.bed << PBJ_LOG_NL
                         << "     Shader ID: " << asset_id_.asset << PBJ_LOG_NL
                         << "  Metadata Key: " << key << PBJ_LOG_NL
                         << "Metadata Value: " << value << PBJ_LOG_END;

        throw std::invalid_argument("Metadata properties may not begin with '__'!");
    }

    if (value.length() == 0)
        metadata_.erase(key);
    else
        metadata_[key] = value;
}

const std::string& Shader::getMetadata(const std::string& key) const
{
    auto i = metadata_.find(key);
    if (i != metadata_.end())
        return i->second;

    return nullString_();
}

const std::map<std::string, std::string>& Shader::getMetadata() const
{
    return metadata_;
}

void Shader::setSource(const std::string& source)
{
    metadata_["__source__"] = source;
    metadata_["__infolog__"].clear();
    invalidate_();
}

const std::string& Shader::getSource() const
{
    return getMetadata("__source__");
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
    return getMetadata("__infolog__");
}

void Shader::compile()
{
    compile_(getSource());
}

std::string& Shader::nullString_() const
{
    static std::string null_str;
    return null_str;
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
                        << "           Bed ID: " << asset_id_.bed << PBJ_LOG_NL
                        << "        Shader ID: " << asset_id_.asset << PBJ_LOG_NL
                        << "      Shader Type: " << (type_ == TVertex ? "Vertex" : (type_ == TFragment ? "Fragment" : "Unknown")) << PBJ_LOG_NL
                        << "   GL Shader Type: " << gl_type << PBJ_LOG_NL
                        << "GL Compile Status: " << result << PBJ_LOG_NL
                        << "      GL Info Log: " << infolog << PBJ_LOG_NL
                        << "    Shader Source: " << source << PBJ_LOG_END;

#ifdef PBJ_EDITOR
        metadata_["__infolog__"] = infolog;
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

        metadata_["__infolog__"] = infolog;
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
