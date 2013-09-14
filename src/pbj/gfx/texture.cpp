///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/texture.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::scene::Texture functions.

#include "pbj/gfx/texture.h"

#include "stb_image.h"

#include <cassert>
#include <iostream>


///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to load a texture from a sandwich.
/// \param  1 The id of the texture.
#define PBJ_GFX_TEXTURE_SQL_LOAD "SELECT data_format, width, height, data, " \
            "internal_format, srgb, mag_filter, min_filter " \
            "FROM sw_textures WHERE id = ?"

#ifdef BE_ID_NAMES_ENABLED
#define PBJ_GFX_TEXTURE_SQLID_LOAD PBJ_GFX_TEXTURE_SQL_LOAD
#else
// TODO: precalculate ids.
#define PBJ_GFX_TEXTURE_SQLID_LOAD PBJ_GFX_TEXTURE_SQL_LOAD
#endif


namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a texture object and uploads it to the GPU.
///
/// \param  id Describes where this texture is stored in the databse.
/// \param  data A pointer to a memory-image of an image file to use for this
///         texture.  The data must be in a format readable by STB Image.
///         eg. PNG, TGA, BMP, JPG, etc.
/// \param  size The number of bytes of data in the data array.
/// \param  format Determines the format used by the GPU to store the texture
///         data on the graphics card.
/// \param  srgb_color If true, the texture will be interpretted as an sRGB
///         encoded image, rather than linearly encoded.  The GPU will
///         automatically linearize the texture data when it is sampled.
/// \param  mag_mode Determines the type of sampler interpolation used when
///         the texture appears larger on-screen than the texture size.
/// \param  min_mode Determines the type of sampler interpolation used when
///         the texture appears smaller on-screen than the texture size.
Texture::Texture(const sw::ResourceId& id, const GLubyte* data, size_t size, InternalFormat format, bool srgb_color, FilterMode mag_mode, FilterMode min_mode)
    : resource_id_(id),
      gl_id_(0)
{
    GLenum error_status;
    while ((error_status = glGetError()) != GL_NO_ERROR)
    {
        PBJ_LOG(VNotice) << "OpenGL error before uploading texture data!" << PBJ_LOG_NL
                         << "    Error Code: " << error_status << PBJ_LOG_NL
                         << "         Error: " << pbj::getGlErrorString(error_status) << PBJ_LOG_END;
    }

    int required_components = 4;

    GLenum internal_format;
    GLenum source_format;
    switch (format)
    {
        case IF_R:
            internal_format = GL_RED;
            source_format = GL_RED;
            required_components = 1;
            break;

        case IF_RG:
            internal_format = GL_RG;
            source_format = GL_RG;
            required_components = 2;
            break;

        case IF_RGB:
            internal_format = srgb_color ? GL_SRGB : GL_RGB;
            source_format = GL_RGB;
            required_components = 3;
            break;

        default: //case IF_RGBA:
            internal_format = srgb_color ? GL_SRGB_ALPHA : GL_RGBA;
            source_format = GL_RGBA;
            required_components = 4;
            break;
    }

    int components;
    stbi_uc* stbi_data = stbi_load_from_memory(data, size, &dimensions_.x, &dimensions_.y, &components, required_components);

    if (stbi_data == nullptr)
    {
        PBJ_LOG(VWarning) << "OpenGL error while parsing texture data!" << PBJ_LOG_NL
                          << "   Sandwich ID: " << resource_id_.sandwich << PBJ_LOG_NL
                          << "    Texture ID: " << resource_id_.resource << PBJ_LOG_NL
                          << "    STBI Error: " << stbi_failure_reason() << PBJ_LOG_END;

        throw std::runtime_error("Failed to upload texture data to GPU!");
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

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, dimensions_.x, dimensions_.y, 0, source_format, GL_UNSIGNED_BYTE, stbi_data);

    stbi_image_free(stbi_data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);

    error_status = glGetError();

    if (error_status != GL_NO_ERROR)
    {
        PBJ_LOG(VWarning) << "OpenGL error while uploading texture data!" << PBJ_LOG_NL
                          << "   Sandwich ID: " << resource_id_.sandwich << PBJ_LOG_NL
                          << "    Texture ID: " << resource_id_.resource << PBJ_LOG_NL
                          << "    Error Code: " << error_status << PBJ_LOG_NL
                          << "         Error: " << pbj::getGlErrorString(error_status) << PBJ_LOG_END;

        throw std::runtime_error("Failed to upload texture data to GPU!");
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  destroys the texture object, ensuring that it is deleted from the
///         GPU.
Texture::~Texture()
{
    if (gl_id_ != 0)
    {
        glDeleteTextures(1, &gl_id_);
        gl_id_ = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves this texture's ResourceId
///
/// \return A ResourceId defining the location of this texture in a database.
const sw::ResourceId& Texture::getId() const
{
    return resource_id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the OpenGL ID of the uploaded texture object.
///
/// \return the OpenGL texture object ID.
GLuint Texture::getGlId() const
{
    return gl_id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the dimensions of the texture.
///
/// \return the texture's dimensions.
const ivec2& Texture::getDimensions() const
{
    return dimensions_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  loads a texture object from a sandwich.
/// 
/// \param  sandwich The database from which to load the texture.
/// \param  texture_id Identifies the texture to load from the database.
std::unique_ptr<Texture> loadTexture(sw::Sandwich& sandwich, const Id& texture_id)
{
    std::unique_ptr<Texture> result;
   
    try
    {
        db::StmtCache& cache = sandwich.getStmtCache();
        db::CachedStmt get_texture = cache.hold(Id(PBJ_GFX_TEXTURE_SQLID_LOAD), PBJ_GFX_TEXTURE_SQL_LOAD);

        get_texture.bind(1, texture_id.value());
        if (get_texture.step())
        {
            Id data_format = Id(get_texture.getUInt64(0));

            if (data_format != Id("stbi"))
                throw std::runtime_error("Unsupported texture data format!");

            const void* data;
            GLsizei data_length = get_texture.getBlob(3, data);

            Texture::InternalFormat internal_format = static_cast<Texture::InternalFormat>(get_texture.getInt(4));
            bool srgb = get_texture.getBool(5);
            Texture::FilterMode mag_mode = static_cast<Texture::FilterMode>(get_texture.getInt(5));
            Texture::FilterMode min_mode = static_cast<Texture::FilterMode>(get_texture.getInt(6));


            result.reset(new Texture(sw::ResourceId(sandwich.getId(), texture_id), static_cast<const GLubyte*>(data), data_length, internal_format, srgb, mag_mode, min_mode));
        }
        else
            throw std::runtime_error("Texture not found!");
    }
    catch (const db::Db::error& err)
    {
        PBJ_LOG(VWarning) << "Database error while loading texture!" << PBJ_LOG_NL
                          << "Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << " Texture ID: " << texture_id << PBJ_LOG_NL
                          << "  Exception: " << err.what() << PBJ_LOG_NL
                          << "        SQL: " << err.sql() << PBJ_LOG_END;
   }
   catch (const std::exception& err)
   {
      PBJ_LOG(VWarning) << "Exception while loading texture!" << PBJ_LOG_NL
                          << "Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << " Texture ID: " << texture_id << PBJ_LOG_NL
                          << "  Exception: " << err.what() << PBJ_LOG_END;
   }

   return result;
}

} // namespace pbj::gfx
} // namespace pbj
