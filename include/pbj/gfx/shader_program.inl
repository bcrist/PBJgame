///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/shader_program.inl
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::gfx::ShaderProgram template functions.

#if !defined(PBJ_GFX_SHADER_PROGRAM_H_) && !defined(DOXYGEN)
#include "pbj/gfx/shader_program.h"
#elif !defined(PBJ_GFX_SHADER_PROGRAM_INL_)
#define PBJ_GFX_SHADER_PROGRAM_INL_

#include <algorithm>

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
ShaderProgram::ShaderProgram(const sw::ResourceId& id,
                             const Iterator& begin,
                             const Iterator& end)
    : resource_id_(id),
      gl_id_(0)
{
    static_assert(std::is_same<std::iterator_traits<Iterator>::value_type, Shader>::value,
        "begin and end must be iterators over Shader objects.");

    handle_.associate(this);

    gl_id_ = glCreateProgram();


    for (Iterator i = begin; i != end; ++i)
        glAttachShader(gl_id_, i->getGlId());

    glLinkProgram(gl_id_);

    checkLinkResult_();
}

} // namespace pbj::gfx
} // namespace pbj

#endif
