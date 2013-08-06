///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/shader_program.h
/// \author Benjamin Crist
///
/// \brief  pbj::gfx::ShaderProgram class header.

#ifndef PBJ_GFX_SHADER_PROGRAM_H_
#define PBJ_GFX_SHADER_PROGRAM_H_

#include "pbj/gfx/shader.h"

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Represents a program consisting of one or more shaders.
///
/// \details Usually a single vertex shader and a single fragment shader are
///         used together.
///
/// \sa     Shader
class ShaderProgram
{
public:
    ShaderProgram(const sw::ResourceId& id, const Shader& vertex_shader, const Shader& fragment_shader);
    template <typename Iterator>
    ShaderProgram(const sw::ResourceId& id, const Iterator& begin, const Iterator& end);
    ~ShaderProgram();

    const be::Handle<ShaderProgram>& getHandle();
    const be::ConstHandle<ShaderProgram>& getHandle() const;

    const sw::ResourceId& getId() const;

    GLuint getGlId() const;

private:
    void checkLinkResult_();
    void invalidate_();

    be::SourceHandle<ShaderProgram> handle_;
    sw::ResourceId resource_id_;

    GLuint gl_id_;

    ShaderProgram(const ShaderProgram&);
    void operator=(const ShaderProgram&);
};

} // namespace pbj::gfx
} // namespace pbj

#include "pbj/gfx/shader_program.inl"

#endif
