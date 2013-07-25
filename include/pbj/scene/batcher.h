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
/// \file   pbj/gfx/batcher.h
/// \author Ben Crist

#ifndef PBJ_GFX_BATCHER_H_
#define PBJ_GFX_BATCHER_H_

#include <functional>
#include <vector>

#include "be/source_handle.h"

#include "pbj/gfx/shader_program.h"
#include "pbj/gfx/texture.h"

#include "pbj/_gl.h"
#include "pbj/_pbj.h"
#include "pbj/_math.h"

namespace pbj {
namespace gfx {

struct SamplerConfig
{
    GLint uniform_location;
    be::ConstHandle<Texture> texture;
};

struct UniformConfig
{
    enum UniformType
    {
        U1f,    // uses glUniform1fv()
        U2f,    // uses glUniform2fv()
        U3f,    // uses glUniform3fv()
        U4f,    // uses glUniform4fv()
        U1i,    // uses glUniform1iv()
        U2i,
        U3i,
        U4i,
        U1u,    // uses glUniform1uiv()
        U2u,
        U3u,
        U4u,
        UM2f,   // uses glUniformMatrix2fv()
        UM3f,
        UM4f,
        UM2x3f, // uses glUniformMatrix2x3fv()
        UM3x2f,
        UM2x4f,
        UM4x2f,
        UM3x4f, 
        UM4x3f
    };

    GLint location;
    UniformType type;
    GLsizei array_size;
    void* data;
};

class Batcher
{
public:
    Batcher();
    ~Batcher();

    void submit(I32 order_index,
                const be::ConstHandle<ShaderProgram>& shader,
                GLuint vao_id,
                GLsizei n_indices,
                GLsizei first_index,
                GLenum index_data_type,
                const SamplerConfig* samplers, size_t n_samplers,
                const UniformConfig* uniforms, size_t n_uniforms,
                bool depth_tested,
                bool scissor_tested,
                ivec2 scissor_pos,
                ivec2 scissor_dim);

    void draw();

private:

    Batcher(const Batcher&);
    void operator=(const Batcher&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
