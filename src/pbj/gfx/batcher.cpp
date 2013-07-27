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

struct TaskCmp
{
    std::vector<BatcherTask>* tasks;

    bool operator()(U16 a_id, U16 b_id)
    {
        BatcherTask* a = &((*tasks)[a_id]);
        BatcherTask* b = &((*tasks)[b_id]);

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
                else if (a->vao_id > b->vao_id)
                    return false;
                else
                {
                    // order index, program, and vao are same

                }*/
            }
        }
    }
};

} // namespace pbj::gfx::(anon)

Batcher::Batcher()
{
}

Batcher::~Batcher()
{
}

void Batcher::submit(const BatcherTask& task)
{
    task_ids_.push_back(tasks_.size());
    tasks_.push_back(task);
}

void Batcher::draw()
{
    // sort batches
    TaskCmp cmp;
    cmp.tasks = &tasks_;
    std::sort(task_ids_.begin(), task_ids_.end(), cmp);

    // keep track of current GL state so we don't respecify stuff
    GLuint current_program = 0;
    GLuint current_vao = 0;
    bool depth_test_enabled = false;
    bool scissor_test_enabled = false;
    ivec2 current_scissor_position;
    ivec2 current_scissor_dimensions;
    
    GLuint texture_units[16];
    for (size_t i = 0; i < 16; ++i)
        texture_units[i] = 0;

    for (U16 index : task_ids_)
    {
        BatcherTask& task = tasks_[index];

        // make sure correct program is bound
        if (task.program_id != current_program)
        {
            current_program = task.program_id;
            glUseProgram(current_program);
        }

        // make sure correct vao is bound
        if (task.vao_id != current_vao)
        {
            current_vao = task.vao_id;
            glBindVertexArray(current_vao);
        }

        // make sure depth testing is enabled/disabled
        if (task.depth_tested != depth_test_enabled)
        {
            depth_test_enabled = task.depth_tested;
            if (depth_test_enabled)
                glEnable(GL_DEPTH_TEST);
            else
                glDisable(GL_DEPTH_TEST);
        }

        // make sure scissor testing is configured properly
        if (task.scissor)
        {
            if (current_scissor_position != task.scissor->position ||
                current_scissor_dimensions != task.scissor->dimensions)
            {
                current_scissor_position = task.scissor->position;
                current_scissor_dimensions = task.scissor->dimensions;

                glScissor(current_scissor_position.x, current_scissor_position.y,
                          current_scissor_dimensions.x, current_scissor_dimensions.y);
            }

            if (!scissor_test_enabled)
            {
                scissor_test_enabled = true;
                glEnable(GL_SCISSOR_TEST);
            }
        }
        else
        {
            if (scissor_test_enabled)
            {
                scissor_test_enabled = false;
                glDisable(GL_SCISSOR_TEST);
            }
        }

        // set up sampler uniforms & textures
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
                case UniformConfig::U1u:        glUniform1uiv(uc.location, uc.array_size, static_cast<const GLuint*>(uc.data)); break;
                case UniformConfig::U2u:        glUniform2uiv(uc.location, uc.array_size, static_cast<const GLuint*>(uc.data)); break;
                case UniformConfig::U3u:        glUniform3uiv(uc.location, uc.array_size, static_cast<const GLuint*>(uc.data)); break;
                case UniformConfig::U4u:        glUniform4uiv(uc.location, uc.array_size, static_cast<const GLuint*>(uc.data)); break;
                case UniformConfig::UM2f:       glUniformMatrix2fv(uc.location, uc.array_size, false, static_cast<const GLfloat*>(uc.data)); break;
                case UniformConfig::UM3f:       glUniformMatrix3fv(uc.location, uc.array_size, false, static_cast<const GLfloat*>(uc.data)); break;
                case UniformConfig::UM4f:       glUniformMatrix4fv(uc.location, uc.array_size, false, static_cast<const GLfloat*>(uc.data)); break;
                case UniformConfig::UM2x3f:     glUniformMatrix2x3fv(uc.location, uc.array_size, false, static_cast<const GLfloat*>(uc.data)); break;
                case UniformConfig::UM3x2f:     glUniformMatrix3x2fv(uc.location, uc.array_size, false, static_cast<const GLfloat*>(uc.data)); break;
                case UniformConfig::UM2x4f:     glUniformMatrix2x4fv(uc.location, uc.array_size, false, static_cast<const GLfloat*>(uc.data)); break;
                case UniformConfig::UM4x2f:     glUniformMatrix4x2fv(uc.location, uc.array_size, false, static_cast<const GLfloat*>(uc.data)); break;
                case UniformConfig::UM3x4f:     glUniformMatrix3x4fv(uc.location, uc.array_size, false, static_cast<const GLfloat*>(uc.data)); break;
                case UniformConfig::UM4x3f:     glUniformMatrix4x3fv(uc.location, uc.array_size, false, static_cast<const GLfloat*>(uc.data)); break;
                default:
                    // TODO: log warning or assert false
                    break;
            }
        }
    
        // Draw VAO elements
        glDrawElements(GL_TRIANGLES, task.n_indices, task.index_data_type, 0);
    }

    if (current_vao != 0)
        glBindVertexArray(0);

    if (current_program != 0)
        glUseProgram(0);

    if (depth_test_enabled)
        glDisable(GL_DEPTH_TEST);

    if (scissor_test_enabled)
        glDisable(GL_SCISSOR_TEST);

    task_ids_.clear();
    tasks_.clear();
}

} // namespace pbj::scene
} // namespace pbj
