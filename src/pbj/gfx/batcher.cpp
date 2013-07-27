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
/// \file   pbj/gfx/batcher.cpp
/// \author Ben Crist

#include "pbj/gfx/batcher.h"

namespace pbj {
namespace gfx {
namespace {

bool task_cmp(const BatcherTask* a, const BatcherTask* b)
{
    if (a->order_index < b->order_index)
        return true;
    else if (a->order_index > b->order_index)
        return false;
    else
    {
        // order indices equal
        // so compare shader programs

        if (a->program_id < b->program_id)
            return true;
        else if (a->program_id > b->program_id)
            return false;
        else
        {
            // order indices and shader programs are the same
            // so compare VAOs

            if (a->vao_id < b->vao_id)
                return true;

            return false;
            /*
            else if (vao_id > other.vao_id)
                return false;
            else
            {
                // order index, program, and vao are same

            }*/
        }
    }
}

} // namespace pbj::gfx::(anon)

Batcher::Batcher()
{
}

Batcher::~Batcher()
{
}

void Batcher::submit(const BatcherTask& task)
{
    tasks_.push_back(task);
    task_ptrs_.push_back(&tasks_.back());
}

void Batcher::draw()
{
    std::sort(task_ptrs_.begin(), task_ptrs_.end(), task_cmp);

    GLuint current_program = 0;
    GLuint current_vao = 0;
    bool depth_test_enabled = false;
    bool scissor_test_enabled = false;
    ivec2 current_scissor_position;
    ivec2 current_scissor_dimensions;
    
    GLuint texture_units[16];
    for (size_t i = 0; i < 16; ++i)
        texture_units[i] = 0;

    for (BatcherTask* ptr : task_ptrs_)
    {
        BatcherTask& task = *ptr;

        if (task.program_id != current_program)
        {
            current_program = task.program_id;
            glUseProgram(current_program);
        }

        if (task.vao_id != current_vao)
        {
            current_vao = task.vao_id;
            glBindVertexArray(current_vao);
        }

        // set up sampler uniforms
        for (size_t i = 0; i < task.n_samplers; ++i)
        {
            const SamplerConfig& sc = task.samplers[i];
            
            size_t tex_unit = 16;
            for (size_t j = 0; j < 16; ++j)
            {
                if (texture_units[j] == sc.texture_id)
                {
                    tex_unit = j;
                    break;
                }
                if (tex_unit >= 16 && texture_units[j] == 0)
                    tex_unit = j;
            }
            
            if (texture_units[tex_unit] != sc.texture_id)
            {
                texture_units[tex_unit] = sc.texture_id;
                glActiveTexture(GL_TEXTURE0 + tex_unit);
                glBindTexture(GL_TEXTURE_2D, sc.texture_id);
            }

            glUniform1i(sc.uniform_location, tex_unit);
        }

        // set up other uniforms
        for (size_t i = 0; i < task.n_uniforms; ++i)
        {
            const UniformConfig& uc = task.uniforms[i];

            switch (uc.type)
            {
                case UniformConfig::U1f:        glUniform1fv(uc.location, uc.array_size, static_cast<const GLfloat*>(uc.data)); break;
                case UniformConfig::U2f:        glUniform2fv(uc.location, uc.array_size, static_cast<const GLfloat*>(uc.data)); break;
                case UniformConfig::U3f:        glUniform3fv(uc.location, uc.array_size, static_cast<const GLfloat*>(uc.data)); break;
                case UniformConfig::U4f:        glUniform4fv(uc.location, uc.array_size, static_cast<const GLfloat*>(uc.data)); break;
                case UniformConfig::U1i:        glUniform1iv(uc.location, uc.array_size, static_cast<const GLint*>(uc.data)); break;
                case UniformConfig::U2i:        glUniform2iv(uc.location, uc.array_size, static_cast<const GLint*>(uc.data)); break;
                case UniformConfig::U3i:        glUniform3iv(uc.location, uc.array_size, static_cast<const GLint*>(uc.data)); break;
                case UniformConfig::U4i:        glUniform4iv(uc.location, uc.array_size, static_cast<const GLint*>(uc.data)); break;
                case UniformConfig::U1f:        glUniform1uiv(uc.location, uc.array_size, static_cast<const GLuint*>(uc.data)); break;
            }
        }
        glUniform4fv(color_uniform_location_, 1, glm::value_ptr(color_));
        glUniformMatrix4fv(transform_uniform_location_, 1, false, glm::value_ptr(transform));

    
        glDrawElements(GL_TRIANGLES, ibo_size_, GL_UNSIGNED_SHORT, 0);
    }

    if (current_vao != 0)
        glBindVertexArray(0);

    if (current_program != 0)
        glUseProgram(0);

    task_ptrs_.clear();
    tasks_.clear();
}

} // namespace pbj::scene
} // namespace pbj
