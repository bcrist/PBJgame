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
/// \file   pbj/gfx/texture.h
/// \author Benjamin Crist
///
/// \brief  pbj::gfx::Texture class header.

#ifndef PBJ_GFX_TEXTURE_H_
#define PBJ_GFX_TEXTURE_H_

#include <map>
#include <vector>

#include "pbj/_pbj.h"
#include "pbj/_math.h"
#include "pbj/_gl.h"
#include "be/source_handle.h"
#include "pbj/sw/resource_id.h"

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Represents an OpenGL texture object
class Texture
{
public:
    enum InternalFormat
    {
        IF_RGBA = 0,
        IF_RGB = 1,
        IF_RG = 2,
        IF_R = 3,
    };

    enum FilterMode
    {
        FM_Linear = 0,
        FM_Nearest = 1
    };

    Texture(const sw::ResourceId& id, const GLubyte* data, size_t size, InternalFormat format, bool srgb_color, FilterMode mag_mode, FilterMode min_mode);
    ~Texture();

    be::Handle<Texture> getHandle();
    const be::ConstHandle<Texture> getHandle() const;

    const sw::ResourceId& getId() const;

    GLuint getGlId() const;

    const ivec2& getDimensions() const;

#ifdef PBJ_EDITOR
    Texture();   // construct without uploading to GL

    void setName(const std::string& name);
    const std::string& getName() const;

    void setSandwich(const Id& id);

    void setMetadata(const std::string& key, const std::string& value);
    const std::string& getMetadata(const std::string& key) const;
    const std::map<std::string, std::string>& getMetadata() const;

    void setData(const GLubyte* data, size_t size);
    size_t getData(const GLubyte*& data) const;

    void setInternalFormat(InternalFormat format);
    InternalFormat getInternalFormat() const;

    void setSrgbColorspace(bool srgb_color);
    bool isSrgbColorspace() const;

    void setMagFilterMode(FilterMode mode);
    FilterMode getMagFilterMode() const;

    void setMinFilterMode(FilterMode mode);
    FilterMode getMinFilterMode() const;

    bool isValid() const;

    void upload();
#endif

private:
    void upload_(const GLubyte* data, size_t size, InternalFormat format, bool srgb_color, FilterMode mag_mode, FilterMode min_mode);
    void invalidate_();

    be::SourceHandle<Texture> handle_;
    sw::ResourceId resource_id_;

    ivec2 dimensions_;
    GLuint gl_id_;
    
#ifdef PBJ_EDITOR
    std::string& nullString_() const;

    std::vector<GLubyte> data_;
    std::map<std::string, std::string> metadata_;
    // special metadata keys:
    // __name__ => asset name
    // __internalformat__ => type of internal representation to use
    // __srgb__ => texture is in SRGB color space, not linear
    // __magfilter__ => filter for magnification
    // __minfilter__ => filter for minification
#endif

    Texture(const Texture&);
    void operator=(const Texture&);
};

} // namespace pbj::gfx
} // namespace pbj

#endif
