///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/shader.h
/// \author Benjamin Crist
///
/// \brief  pbj::gfx::Shader class header.

#ifndef PBJ_GFX_SHADER_H_
#define PBJ_GFX_SHADER_H_

#include <map>

#include "pbj/_pbj.h"
#include "pbj/_gl.h"
#include "pbj/sw/sandwich.h"
#include "pbj/sw/resource_id.h"

#include <memory>

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Represents a compiled Vertex/Fragment Shader
///
/// \sa     ShaderProgram
class Shader
{
public:
    enum Type
    {
        TVertex,
        TFragment
    };

    Shader(const sw::ResourceId& id, Type type, const std::string& source);
    ~Shader();

    const sw::ResourceId& getId() const;

    Type getType() const;
    GLuint getGlId() const;

private:
    void compile_(const std::string& source);
    void checkCompileResult_(const std::string& source);
    void invalidate_();

    sw::ResourceId id_;

    GLuint gl_id_;
    Type type_;

    Shader(const Shader&);
    void operator=(const Shader&);
};

std::unique_ptr<Shader> loadShader(sw::Sandwich& sandwich, const Id& shader_id);

} // namespace pbj::gfx
} // namespace pbj

#endif
