// Copyright (c) 2013 PBJ^2 Productions
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

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
