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

#include "pbj/sw/sandwich.h"

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

std::unique_ptr<Texture> loadTexture(sw::Sandwich& sandwich, const Id& texture_id);



} // namespace pbj::gfx
} // namespace pbj

#endif
