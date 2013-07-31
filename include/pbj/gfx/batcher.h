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
    GLuint texture_id;
};

struct UniformConfig
{
	//The values assigned are what are returned from glGetActiveUniformsiv
	//with a parameter of GL_UNIFORM_TYPE.  This is for the convenience of
	//casting -- Peter
    enum UniformType
    {
        U1f = GL_FLOAT,    // uses glUniform1fv()
        U2f = GL_FLOAT_VEC2,    // uses glUniform2fv()
        U3f = GL_FLOAT_VEC3,    // uses glUniform3fv()
        U4f = GL_FLOAT_VEC4,    // uses glUniform4fv()
        U1i = GL_INT,    // uses glUniform1iv()
        U2i = GL_INT_VEC2,
        U3i = GL_INT_VEC3,
        U4i = GL_INT_VEC4,
        U1u = GL_UNSIGNED_INT,    // uses glUniform1uiv()
        U2u = GL_UNSIGNED_INT_VEC2,
        U3u = GL_UNSIGNED_INT_VEC3,
        U4u = GL_UNSIGNED_INT_VEC4,
        UM2f = GL_FLOAT_MAT2,   // uses glUniformMatrix2fv()
        UM3f = GL_FLOAT_MAT3,
        UM4f = GL_FLOAT_MAT4,
        UM2x3f = GL_FLOAT_MAT2x3, // uses glUniformMatrix2x3fv()
        UM3x2f = GL_FLOAT_MAT3x2,
        UM2x4f = GL_FLOAT_MAT2x4,
        UM4x2f = GL_FLOAT_MAT4x2,
        UM3x4f = GL_FLOAT_MAT3x4, 
        UM4x3f = GL_FLOAT_MAT4x3
    };

    GLint location;
    UniformType type;
    GLsizei array_size;
    const void* data;
};

struct ScissorConfig
{
    ivec2 position;
    ivec2 dimensions;
};

struct BatcherTask
{
    I32 order_index;
    GLuint program_id;
    GLuint vao_id;
    GLsizei n_indices;
    GLenum index_data_type;
    const SamplerConfig* samplers;
    size_t n_samplers;
    const UniformConfig* uniforms;
    size_t n_uniforms;
    bool depth_tested;
    const ScissorConfig* scissor;
};

class Batcher
{
public:
    Batcher();
    ~Batcher();

    void submit(const BatcherTask& task);

    void draw();

private:
    std::vector<BatcherTask> tasks_;
    std::vector<U16> task_ids_;

    Batcher(const Batcher&);
    void operator=(const Batcher&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
