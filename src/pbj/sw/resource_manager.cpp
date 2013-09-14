///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/sw/resource_manager.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::sw::ResourceManager functions.

#include "pbj/sw/resource_manager.h"

#include "pbj/gfx/texture_font.h"
#include "pbj/gfx/mesh.h"
#include "pbj/gfx/texture.h"
#include "pbj/gfx/shader.h"
#include "pbj/gfx/shader_program.h"
#include "pbj/sw/sandwich_open.h"
#include "pbj/_gl.h"

#include <iostream>

namespace pbj {
namespace sw {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs an empty ResourceManager.
ResourceManager::ResourceManager()
{
    
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destroys this ResourceManager, along with all resources owned by
///         the manager.
ResourceManager::~ResourceManager()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a TextureFont from this ResourceManager, loading it from
///         a sandwich if necessary.
///
/// \param  id The ResourceId of the asset to retrieve.
/// \return A reference to the requested resource.
/// \throws std::invalid_argument If the TextureFont could not be loaded.
const gfx::TextureFont& ResourceManager::getTextureFont(const ResourceId& id)
{
    auto i = texture_fonts_.find(id);
    if (i != texture_fonts_.end())
        return *i->second;

    // if we get to here, the resource is not loaded yet.
    Sandwich& sandwich = getSandwich(id.sandwich);

    std::unique_ptr<gfx::TextureFont> ptr = gfx::loadTexture(sandwich, id.resource);

    if (ptr)
    {
        gfx::TextureFont* font = ptr.get();
        texture_fonts_[id] = std::move(ptr);
        return *font;
    }

    // if we get to here, the resource could not be loaded from the sandwich
    PBJ_LOG(VError) << "TextureFont not found!" << PBJ_LOG_NL
                    << "   Sandwich ID: " << id.sandwich << PBJ_LOG_NL
                    << "TextureFont ID: " << id.resource << PBJ_LOG_END;

    throw std::invalid_argument("TextureFont not found!");
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a Mesh from this ResourceManager, loading it from a
///         sandwich if necessary.
///
/// \param  id The :ResourceId of the mesh to retrieve.
/// \return A reference to the requested resource.
/// \throws std::invalid_argument If the Mesh could not be loaded.
const gfx::Mesh& ResourceManager::getMesh(const ResourceId& id)
{
    auto i = meshes_.find(id);
    if (i != meshes_.end())
        return *i->second;

    // if we get to here, the resource is not loaded yet.
    Sandwich& sandwich = getSandwich(id.sandwich);

    std::unique_ptr<gfx::Mesh> ptr = gfx::loadMesh(sandwich, id.resource);

    if (ptr)
    {
        gfx::Mesh* mesh = ptr.get();
        meshes_[id] = std::move(ptr);
        return *mesh;
    }

    // if we get to here, the resource could not be loaded from the sandwich
    PBJ_LOG(VError) << "Mesh not found!" << PBJ_LOG_NL
                    << "Sandwich ID: " << id.sandwich << PBJ_LOG_NL
                    << "    Mesh ID: " << id.resource << PBJ_LOG_END;

    throw std::invalid_argument("Mesh not found!");
}
    
///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a Texture from this ResourceManager, loading it from a
///         sandwich if necessary.
///
/// \param  id The ResourceId of the texture to retrieve.
/// \return A reference to the requested resource.
/// \throws std::invalid_argument If the Texture could not be loaded.
const gfx::Texture& ResourceManager::getTexture(const ResourceId& id)
{
    auto i = textures_.find(id);
    if (i != textures_.end())
        return *i->second;

    // if we get to here, the resource is not loaded yet.
    Sandwich& sandwich = getSandwich(id.sandwich);

    std::unique_ptr<gfx::Texture> ptr = gfx::loadTexture(sandwich, id.resource);

    if (ptr)
    {
        gfx::Texture* tex = ptr.get();
        textures_[id] = std::move(ptr);
        return *tex;
    }

    // if we get to here, the resource could not be loaded from the sandwich
    PBJ_LOG(VError) << "Texture not found!" << PBJ_LOG_NL
                    << "Sandwich ID: " << id.sandwich << PBJ_LOG_NL
                    << " Texture ID: " << id.resource << PBJ_LOG_END;

    throw std::invalid_argument("Texture not found!");
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a Shader from this ResourceManager, loading it from a
///         sandwich if necessary.
///
/// \param  id The ResourceId of the shader to retrieve.
/// \return A reference to the requested resource.
/// \throws std::invalid_argument If the Shader could not be loaded.
const gfx::Shader& ResourceManager::getShader(const ResourceId& id)
{
    auto i = shaders_.find(id);
    if (i != shaders_.end())
        return *i->second;

    // if we get to here, the resource is not loaded yet.
    Sandwich& sandwich = getSandwich(id.sandwich);

    std::unique_ptr<gfx::Shader> ptr = gfx::loadShader(sandwich, id.resource);

    if (ptr)
    {
        gfx::Shader* shader = ptr.get();
        shaders_[id] = std::move(ptr);
        return *shader;
    }

    // if we get to here, the resource could not be loaded from the sandwich
    PBJ_LOG(VError) << "Shader not found!" << PBJ_LOG_NL
                    << "Sandwich ID: " << id.sandwich << PBJ_LOG_NL
                    << "  Shader ID: " << id.resource << PBJ_LOG_END;

    throw std::invalid_argument("Shader not found!");
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a ShaderProgram from this ResourceManager, loading it
///         from a sandwich if necessary.
///
/// \param  id The ResourceId of the program to retrieve.
/// \return A reference to the requested resource.
/// \throws std::invalid_argument If the ShaderProgram could not be loaded
const gfx::ShaderProgram& ResourceManager::getProgram(const ResourceId& id)
{
    auto i = programs_.find(id);
    if (i != programs_.end())
        return *i->second;

    // if we get to here, the resource is not loaded yet.
    Sandwich& sandwich = getSandwich(id.sandwich);

    std::unique_ptr<gfx::ShaderProgram> ptr = gfx::loadShaderProgram(sandwich, id.resource);

    if (ptr)
    {
        gfx::ShaderProgram* prog = ptr.get();
        programs_[id] = std::move(ptr);
        return *prog;
    }

    // if we get to here, the resource could not be loaded from the sandwich
    PBJ_LOG(VError) << "ShaderProgram not found!" << PBJ_LOG_NL
                    << "
                    << "ShaderProgram ID: " << id << PBJ_LOG_END;

    throw std::invalid_argument("Program not found!");
}

} // namespace pbj::gfx
} // namespace pbj
