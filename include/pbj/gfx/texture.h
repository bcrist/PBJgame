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

    Texture(const GLubyte* data, size_t size, InternalFormat format, bool srgb_color, FilterMode mag_mode, FilterMode min_mode);
    ~Texture();

    GLuint getGlId() const;
    const ivec2& getDimensions() const;

    void bind() const;
    static void unbind();

private:
    ivec2 dimensions_;
    GLuint gl_id_;
    
    Texture(const Texture&);
    void operator=(const Texture&);
};

std::unique_ptr<Texture> loadTexture(sw::Sandwich& sandwich, const Id& texture_id);

} // namespace pbj::gfx
} // namespace pbj

#endif
