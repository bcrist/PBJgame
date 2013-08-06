///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/built_ins.h
/// \author Benjamin Crist
///
/// \brief  pbj::gfx::BuiltIns class header.

#ifndef PBJ_GFX_BUILT_INS_H_
#define PBJ_GFX_BUILT_INS_H_

#include "pbj/sw/resource_id.h"
#include "pbj/_pbj.h"

#include <unordered_map>
#include <memory>

namespace pbj {

class Engine;

namespace gfx {

class TextureFont;

class Mesh;
class Texture;
class Shader;
class ShaderProgram;

class BuiltIns
{
    friend class Engine;
    friend struct std::default_delete<BuiltIns>;

public:
    const TextureFont& getTextureFont(const Id& id) const;

    const Mesh& getMesh(const Id& id) const;
    
    const Texture& getTexture(const Id& id) const;

    const Shader& getShader(const Id& id) const;
    const ShaderProgram& getProgram(const Id& id) const;

private:
    BuiltIns();
    ~BuiltIns();

    void logWarning(const char* type, const sw::ResourceId id, const std::string& what_arg) const;

    std::unordered_map<Id, std::unique_ptr<TextureFont> > texture_fonts_;

    std::unordered_map<Id, std::unique_ptr<Mesh> > meshes_;

    std::unordered_map<Id, std::unique_ptr<Texture> > textures_;

    std::unordered_map<Id, std::unique_ptr<Shader> > shaders_;
    std::unordered_map<Id, std::unique_ptr<ShaderProgram> > programs_;


    BuiltIns(const BuiltIns&);
    void operator=(const BuiltIns&);
};

} // namespace pbj::gfx
} // namespace pbj

#endif
