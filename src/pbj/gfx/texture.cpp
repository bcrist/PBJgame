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

///////////////////////////////////////////////////////////////////////////////
/// \brief  destroys the texture object, ensuring that it is deleted from the
///         GPU.
Texture::~Texture()
{
    invalidate_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns a handle to this texture.
///
/// \return A Handle<Texture> referring to this texture object.
be::Handle<Texture> Texture::getHandle()
{
    return handle_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns a handle to this texture.
///
/// \return A Handle<Texture> referring to this texture object.
const be::ConstHandle<Texture> Texture::getHandle() const
{
    return handle_;
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

#ifdef PBJ_EDITOR
///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a texture without uploading it to the GPU.
///
/// \details only available in the editor.
Texture::Texture()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets this texture's name.
///
/// \details The texture name is not used outside the editor, but it also
///         determines the Id of the texture used to store it in a sandwich.
///
/// \param  name The new name for the texture.
void Texture::setName(const std::string& name)
{
    metadata_["__name__"] = name;
    resource_id_.resource = Id(name);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the texture's name.
/// 
/// \details The texture name is only available in the editor, as it is stored
///         as editor metadata.
///
/// \return The current name of the texture.
const std::string& Texture::getName() const
{
    return getMetadata("__name__");
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the sandwich where this texture should be stored.
///
/// \param  id the Id of the sandwich to store this texture in.
void Texture::setSandwich(const Id& id)
{
    resource_id_.sandwich = id;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets a metadata key-value pair.  (i.e. author, date, version, etc)
///
/// \param  key The metadata property to set.
/// \param  value The new value of the metadata property.
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

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a metadata value.
///
/// \param  key The metadata property to get.
///
/// \return The value of the requested property.
const std::string& Texture::getMetadata(const std::string& key) const
{
    auto i = metadata_.find(key);
    if (i != metadata_.end())
        return i->second;

    return nullString_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves all the metadata associated with this texture.
///
/// \return a const view of the metadata map associated with this
///         texture.
const std::map<std::string, std::string>& Texture::getMetadata() const
{
    return metadata_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the texture data associated with this texture.
///
/// \details If the texture has already been uploaded to the GPU, it will be
///         invalidated and deleted from the GPU.
///
/// \param  data A pointer to the new texture data.
/// \param  size The number of bytes in the new texture data.
void Texture::setData(const GLubyte* data, size_t size)
{
    data_.clear();
    data_.reserve(size);
    memcpy(data_.data(), data, sizeof(GLubyte) * size);
    
    invalidate_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves this texture's data.
///
/// \param  data a reference to a pointer which will be set to the beginnning
///         of the texture data.
/// \return The size of the data byte array.
size_t Texture::getData(const GLubyte*& data) const
{
    data = data_.data();
    return data_.size();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the format used internally to store texture data on the GPU.
///
/// \param  format The internal format to use.
void Texture::setInternalFormat(InternalFormat format)
{
    metadata_["__internalformat__"] = std::to_string(format);

    invalidate_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the format used to store the texture data on the GPU.
///
/// \return The current internal format.
Texture::InternalFormat Texture::getInternalFormat() const
{
    return static_cast<InternalFormat>(std::stoi(getMetadata("__internalformat__")));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets whether or not the texture data is interpretted using an sRGB
///         or linear color space.
///
/// \param  srgb_color true if the texture should be interpretted using the
///         srgb color space.
void Texture::setSrgbColorspace(bool srgb_color)
{
    metadata_["__srgb__"] = srgb_color ? "1" : "0";

    invalidate_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns whether or not the texture data is interpretted using an
///         sRGB or linear color space.
///
/// \return true if the texture is interpretted using the srgb color space.
bool Texture::isSrgbColorspace() const
{
    return std::stoi(getMetadata("__srgb__")) != 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the interpolation filter mode for magnification.
///
/// \param  mode The magnification interpolation filter mode.
void Texture::setMagFilterMode(FilterMode mode)
{
    metadata_["__minfilter__"] = std::to_string(mode);

    invalidate_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the interpolation filter mode for magnification.
///
/// \return The magnification interpolation filter mode.
Texture::FilterMode Texture::getMagFilterMode() const
{
    return static_cast<FilterMode>(std::stoi(getMetadata("__magfilter__")));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the interpolation filter mode for minification.
///
/// \param  mode The minification interpolation filter mode.
void Texture::setMinFilterMode(FilterMode mode)
{
    metadata_["__minfilter__"] = std::to_string(mode);

    invalidate_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the interpolation filter mode for minification.
///
/// \return The minification interpolation filter mode.
Texture::FilterMode Texture::getMinFilterMode() const
{
    return static_cast<FilterMode>(std::stoi(getMetadata("__minfilter__")));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Determines whether the texture has been successfully uploaded to
///         the GPU.
///
/// \return true if the texture is valid and can be used for drawing.
bool Texture::isValid() const
{
    return gl_id_ != 0;
}   

///////////////////////////////////////////////////////////////////////////////
/// \brief  Uploads the current texture to the GPU.
void Texture::upload()
{   
    upload_(data_.data(), data_.size(), getInternalFormat(), isSrgbColorspace(), getMagFilterMode(), getMinFilterMode());
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns a reference to a static, empty string object.
///
/// \return A reference to a static, empty string object.
std::string& Texture::nullString_() const
{
    static std::string null_str;
    return null_str;
}

#endif

///////////////////////////////////////////////////////////////////////////////
/// \brief  Helper for uploading texture data to the GPU.
///
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
void Texture::upload_(const GLubyte* data, size_t size, InternalFormat format, bool srgb_color, FilterMode mag_mode, FilterMode min_mode)
{
    invalidate_();

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

        invalidate_();

        throw std::runtime_error("Failed to upload texture data to GPU!");
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  invalidates this texture, deleting it from GPU memory.
void Texture::invalidate_()
{
    if (gl_id_ != 0)
    {
        glDeleteTextures(1, &gl_id_);
        gl_id_ = 0;
    }
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
