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
/// \file   pbj/scene/texture.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::scene::Texture functions.

#include "pbj/scene/texture.h"

#include "stb_image.h"

#include <cassert>
#include <iostream>

namespace pbj {
namespace scene {

Texture::Texture(const sw::ResourceId& id, const GLubyte* data, size_t size, InternalFormat format, bool srgb_color, FilterMode mag_mode, FilterMode min_mode)
    : resource_id_(id),
      gl_id_(0)
{
    handle_.associate(this);

#ifdef PBJ_EDITOR
    setData(data, size);
    setInternalFormat(format);
    setSrgbColorspace(srgb_color);
    setMagFilterMode(mag_mode);
    setMinFilterMode(min_mode);
#endif

    upload_(data, size, format, srgb_color, mag_mode, min_mode);
}

Texture::~Texture()
{
    invalidate_();
}

be::Handle<Texture> Texture::getHandle()
{
    return handle_;
}

const be::ConstHandle<Texture> Texture::getHandle() const
{
    return handle_;
}

const sw::ResourceId& Texture::getId() const
{
    return resource_id_;
}

GLuint Texture::getGlId() const
{
    return gl_id_;
}

#ifdef PBJ_EDITOR
Texture::Texture()
{
}

void Texture::setName(const std::string& name)
{
    metadata_["__name__"] = name;
    resource_id_.resource = Id(name);
}

const std::string& Texture::getName() const
{
    return getMetadata("__name__");
}

void Texture::setSandwich(const Id& id)
{
    resource_id_.sandwich = id;
}

void Texture::setMetadata(const std::string& key, const std::string& value)
{
    if (key[0] == '_' && key[1] == '_')
    {
        PBJ_LOG(VNotice) << "Attempted to set invalid metadata property!" << PBJ_LOG_NL
                         << "   Sandwich ID: " << resource_id_.sandwich << PBJ_LOG_NL
                         << "    Texture ID: " << resource_id_.resource << PBJ_LOG_NL
                         << "  Metadata Key: " << key << PBJ_LOG_NL
                         << "Metadata Value: " << value << PBJ_LOG_END;

        throw std::invalid_argument("Metadata properties may not begin with '__'!");
    }

    if (value.length() == 0)
        metadata_.erase(key);
    else
        metadata_[key] = value;
}

const std::string& Texture::getMetadata(const std::string& key) const
{
    auto i = metadata_.find(key);
    if (i != metadata_.end())
        return i->second;

    return nullString_();
}

const std::map<std::string, std::string>& Texture::getMetadata() const
{
    return metadata_;
}

void Texture::setData(const GLubyte* data, size_t size)
{
    data_.clear();
    data_.reserve(size);
    memcpy(data_.data(), data, sizeof(GLubyte) * size);
    
    invalidate_();
}

size_t Texture::getData(const GLubyte*& data) const
{
    data = data_.data();
    return data_.size();
}

void Texture::setInternalFormat(InternalFormat format)
{
    metadata_["__internalformat__"] = std::to_string(format);

    invalidate_();
}

Texture::InternalFormat Texture::getInternalFormat() const
{
    return static_cast<InternalFormat>(std::stoi(getMetadata("__internalformat__")));
}

void Texture::setSrgbColorspace(bool srgb_color)
{
    metadata_["__srgb__"] = srgb_color ? "1" : "0";

    invalidate_();
}

bool Texture::isSrgbColorspace() const
{
    return std::stoi(getMetadata("__srgb__")) != 0;
}

void Texture::setMagFilterMode(FilterMode mode)
{
    metadata_["__minfilter__"] = std::to_string(mode);

    invalidate_();
}

Texture::FilterMode Texture::getMagFilterMode() const
{
    return static_cast<FilterMode>(std::stoi(getMetadata("__magfilter__")));
}

void Texture::setMinFilterMode(FilterMode mode)
{
    metadata_["__minfilter__"] = std::to_string(mode);

    invalidate_();
}

Texture::FilterMode Texture::getMinFilterMode() const
{
    return static_cast<FilterMode>(std::stoi(getMetadata("__minfilter__")));
}

bool Texture::isValid() const
{
    return gl_id_ != 0;
}   

void Texture::upload()
{   
    upload_(data_.data(), data_.size(), getInternalFormat(), isSrgbColorspace(), getMagFilterMode(), getMinFilterMode());
}

std::string& Texture::nullString_() const
{
    static std::string null_str;
    return null_str;
}

#endif

void Texture::upload_(const GLubyte* data, size_t size, InternalFormat format, bool srgb_color, FilterMode mag_mode, FilterMode min_mode)
{
    invalidate_();

    int components;
    stbi_uc* stbi_data = stbi_load_from_memory(data, size, &dimensions_.x, &dimensions_.y, &components, 4);

    if (stbi_data == nullptr)
    {
        PBJ_LOG(VWarning) << "OpenGL error while parsing texture data!" << PBJ_LOG_NL
                          << "   Sandwich ID: " << resource_id_.sandwich << PBJ_LOG_NL
                          << "    Texture ID: " << resource_id_.resource << PBJ_LOG_NL
                          << "    STBI Error: " << stbi_failure_reason() << PBJ_LOG_END;

        throw std::runtime_error("Failed to upload texture data to GPU!");
    }

    GLenum internal_format;
    switch (format)
    {
        case IF_R:
            internal_format = GL_RED;
            break;

        case IF_RG:
            internal_format = GL_RG;
            break;

        case IF_RGB:
            internal_format = srgb_color ? GL_SRGB : GL_RGB;
            break;

        case IF_RGBA:
            internal_format = srgb_color ? GL_SRGB_ALPHA : GL_RGBA;
            break;

        default:
            internal_format = GL_RGBA;
            break;
    }

    GLenum mag_filter;
    GLenum min_filter;
    switch (mag_mode)
    {
        case FM_Linear:     mag_filter = GL_LINEAR; break;
        case FM_Nearest:    mag_filter = GL_NEAREST; break;
        default:            mag_filter = GL_LINEAR; break;
    }

    switch (min_mode)
    {
        case FM_Linear:     min_filter = GL_LINEAR; break;
        case FM_Nearest:    min_filter = GL_NEAREST; break;
        default:            min_filter = GL_LINEAR; break;
    }


    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &gl_id_);
    glBindTexture(GL_TEXTURE_2D, gl_id_);

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, dimensions_.x, dimensions_.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(stbi_data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);

    GLenum error_status = glGetError();

    if (error_status != GL_NO_ERROR)
    {
        PBJ_LOG(VWarning) << "OpenGL error while uploading texture data!" << PBJ_LOG_NL
                          << "   Sandwich ID: " << resource_id_.sandwich << PBJ_LOG_NL
                          << "    Texture ID: " << resource_id_.resource << PBJ_LOG_NL
                          << "    Error Code: " << error_status << PBJ_LOG_NL
                          << "         Error: " << pbj::getGlErrorString(error_status) << PBJ_LOG_END;

        invalidate_();

        throw std::runtime_error("Failed to upload texture data to GPU!");
    }
}

void Texture::invalidate_()
{
    if (gl_id_ != 0)
    {
        glDeleteTextures(1, &gl_id_);
        gl_id_ = 0;
    }
}

} // namespace pbj::scene
} // namespace pbj
