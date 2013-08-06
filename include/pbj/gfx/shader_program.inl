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
/// \brief  Constructs a shader program by iterating over a container of
///         Shader objects or pointers to Shader objects.
///
/// \param  id Describes where this shader program is stored in the databse.
/// \param  begin An iterator over Shaders or pointers to Shaders pointing to
///         the first shader associated with this program.
/// \param  end An iterator over Shaders or pointers to Shaders pointing to
///         a point just after the last shader associated with this program.
template <typename Iterator>
ShaderProgram::ShaderProgram(const sw::ResourceId& id,
                             const Iterator& begin,
                             const Iterator& end)
    : resource_id_(id),
      gl_id_(0)
{
    static_assert(std::is_same<std::iterator_traits<Iterator>::value_type, Shader>::value ||
                  std::is_same<std::iterator_traits<Iterator>::value_type, Shader*>::value,
                  "begin and end must be iterators over Shader objects.");

    handle_.associate(this);

    gl_id_ = glCreateProgram();

    for (Iterator i = begin; i != end; ++i)
    {
        if (std::is_same<std::iterator_traits<Iterator>::value_type, Shader>::value)
        {
            // Iterator is over Shader objects
            glAttachShader(gl_id_, i->getGlId());
        }
        else 
        {
            // Iterator is over Shader* pointers
            glAttachShader(gl_id_, (*i)->getGlId());
        }
    }

    glLinkProgram(gl_id_);

    checkLinkResult_();
}

} // namespace pbj::gfx
} // namespace pbj

#endif
