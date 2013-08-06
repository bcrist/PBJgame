///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/batcher.h
/// \author Ben Crist

#ifndef PBJ_GFX_BATCHER_H_
#define PBJ_GFX_BATCHER_H_

#include <functional>
#include <vector>
#include <random>

#include "be/source_handle.h"

#include "pbj/gfx/shader_program.h"
#include "pbj/gfx/texture.h"
#include "pbj/_math.h"

#include "pbj/_gl.h"
#include "pbj/_pbj.h"
#include "pbj/_math.h"

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Describes the texture that should be bound to a particular GLSL
///         sampler uniform.
struct SamplerConfig
{
    GLint uniform_location; ///< The location of the GLSL uniform, retrieved using glGetUniformLocation().
    GLuint texture_id;      ///< The OpenGL texture object ID of the texture that should be bound to the sampler.
};

///////////////////////////////////////////////////////////////////////////////
/// \brief  Describes the data that should be bound to a non-sampler GLSL
///         uniform.
struct UniformConfig
{
    ///////////////////////////////////////////////////////////////////////////
    /// \brief  The data type of the uniform.
    ///
    /// \details The number indicates the size of vector or matrix, while the
    ///         trailing "f", "i", or "u" indicates if components are floats,
    ///         ints, or unsigned ints.
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

    GLint location;     ///< The location of the uniform, retrieved using glGetUniformLocation()
    UniformType type;   ///< The data type of the uniform, as described above.
    GLsizei array_size; ///< If the uniform is an array, the number of elements in the array.  Note that a single vector or matrix is NOT considered an array.
    const void* data;   ///< A pointer to the current uniform's data.
};

///////////////////////////////////////////////////////////////////////////////
/// \brief  Defines a screen-space rectangle to clip drawing to.
struct ScissorConfig
{
    ivec2 position;     ///< The viewport coordinates of the lower-left corner of the clip rectangle.  Coordinates increase to the right and up.
    ivec2 dimensions;   ///< The dimensions of the scissor box in viewport coordinates.
};

///////////////////////////////////////////////////////////////////////////////
/// \brief  Defines all the information required draw a series of triangles.
///
/// \details BatcherTasks are always drawn using glDrawElements(), not
///         glDrawArrays() so VAOs must have an associated index buffer (IBO)
///         in addition to a VBO.
struct BatcherTask
{
    I32 order_index;                ///< Primary task sorting index.  Allows for manual painter's algorithm ordering.
    GLuint program_id;              ///< The OpenGL ID of the shader program to use.  All tasks which use the same program and have the same order_index will be rendered together.
    GLuint vao_id;                  ///< The OpenGL ID of the VAO to use.  The VAO stores information about the vertex buffer(s) and index buffer to use.
    GLsizei n_indices;              ///< The number of indices in the VAO's index buffer.  Drawing is always started from the first index.
    GLenum index_data_type;         ///< The data type of the elements in the IBO (one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT).
    const SamplerConfig* samplers;  ///< A pointer to an array of SamplerConfig objects describing the texture bindings used by this drawing task.
    size_t n_samplers;              ///< The size of the array pointed to by samplers.
    const UniformConfig* uniforms;  ///< A pointer to an array of UniformConfig objects describing the current values non-sampler uniforms used by this drawing task.
    size_t n_uniforms;              ///< The size of the array pointed to by uniforms.
    bool depth_tested;              ///< A flag indicating whether fragments should be depth tested.
    const ScissorConfig* scissor;   ///< A pointer to a ScissorConfig object to restrict the drawing area, or nullptr if no scissor testing should be performed.
};

///////////////////////////////////////////////////////////////////////////////
/// \brief  Provides a facility to execute drawing tasks efficiently, by
///         minimizing changes to expensive OpenGL state, like shader programs,
///         VAOs, and textures
class Batcher
{
public:
    Batcher();
    ~Batcher();

    void submit(const BatcherTask& task);

    void draw();

private:
    static std::mt19937 prng_;
    static std::uniform_int_distribution<I32> idist_;
    

    std::vector<BatcherTask> tasks_;
    std::vector<U16> task_ids_;

    Batcher(const Batcher&);
    void operator=(const Batcher&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
