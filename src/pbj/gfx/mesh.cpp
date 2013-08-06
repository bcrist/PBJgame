///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/mesh.h
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::gfx::Mesh functions.

#include "pbj/gfx/mesh.h"

#include <iostream>

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a MeshVertexAttributeSpec with no attributes enabled.
MeshVertexAttributeSpec::MeshVertexAttributeSpec()
{
    for (int i = 0; i < 16; ++i)
        indices[i] = -1;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new mesh and uploads it's data to the GPU.
///
/// \details The mesh data is encompassed in an OpenGL Vertex Array Object
///         (VAO) which holds the configuration of a Vertex Buffer Object (VBO)
///         and an index buffer object (IBO) necessary to render the mesh.
///
/// \param  id Describes the location of the mesh in the database it was loaded
///         from.
/// \param  vertex_data A pointer to the beginning of an array of vec4 data
///         which describes the vertices in the mesh.  Each vertex may span
///         multiple vec4 "slots" if there is data other than positions
/// \param  attributes A MeshVertexAttributes object which describes how the
///         vec4s in vertex_data are bound to GLSL attribute inputs.
/// \param  n_vertices The number of vertices in the mesh.  If vertex_size
///         is defined as the number of indices from the attributes parameter
///         before the first negative index, then
///         vertex_data[(n_vertices * vertex_size) - 1] will be the start of
///         the last vertex in vertex_data.
/// \param  index_data An array of unsigned 16-bit unsigned integers
///         representing the order that vertices are rendered.  Vertices
///         can be used more than once by repeating it's index.
/// \param  n_indices The number of indices in index_data.
Mesh::Mesh(const sw::ResourceId& id,
           vec4* vertex_data,
           const MeshVertexAttributeSpec& attributes,
           GLsizei n_vertices,
           GLushort* index_data,
           GLsizei n_indices)
    : resource_id_(id),
      index_count_(n_indices)
{
    handle_.associate(this);

    GLsizei index_data_size = n_indices * sizeof(GLushort);
   
    // Generate VAO for mesh
    glGenVertexArrays(1, &vao_id_);
    glBindVertexArray(vao_id_);             // bind VAO

    // Calculate size of 1 vertex (incl. all vertex attributes)
    // GL calls this "stride"
    size_t n_attributes = 0;
    for (size_t i = 0; i < 16; ++i)
    {
        if (attributes.indices[i] < 0)
            break;
        else
            n_attributes++;
    }
    GLsizei stride = n_attributes * sizeof(vec4);

    // Generate & fill IBO
    glGenBuffers(1, &ibo_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id_); // bind IBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data_size, index_data, GL_STATIC_DRAW);
    // don't unbind IBO because it is part of the VAO's state.

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        PBJ_LOG(pbj::VWarning) << "OpenGL error!" << PBJ_LOG_NL
                               << "Error Code: " << err << PBJ_LOG_NL
                               << "     Error: " << pbj::getGlErrorString(err) << PBJ_LOG_END;
    }

    // Generate & fill VBO
    glGenBuffers(1, &vbo_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_); // bind VBO
    glBufferData(GL_ARRAY_BUFFER, stride * n_vertices, vertex_data, GL_STATIC_DRAW);
    
    // set up vertex attribute pointers & enable them
    for (size_t i = 0; i < n_attributes; ++i)
    {
        GLuint attribute_index = attributes.indices[i];
        glVertexAttribPointer(attribute_index, 4, GL_FLOAT, false, stride, reinterpret_cast<void*>(i * sizeof(vec4)));
        glEnableVertexAttribArray(attribute_index);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);       // unbind VBO

    glBindVertexArray(0);                   // unbind VAO
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destroys the mesh, destroying the VAO, VBO, and IBO from the GPU.
Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &vao_id_);
    glDeleteBuffers(1, &vbo_id_);
    glDeleteBuffers(1, &ibo_id_);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a handle to this mesh.
///
/// \return A Handle<Mesh> referring to this mesh object.
const be::Handle<Mesh>& Mesh::getHandle()
{
    return handle_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a handle to this mesh.
///
/// \return A ConstHandle<Mesh> referring to this mesh object.
const be::ConstHandle<Mesh>& Mesh::getHandle() const
{
    return handle_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves this mesh's ResourceId
///
/// \return A ResourceId defining the location of this mesh in a database.
const sw::ResourceId& Mesh::getId() const
{
    return resource_id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the OpenGL ID of the Vertex Array Object for this mesh.
///
/// \return The ID for this mesh's VAO.
GLuint Mesh::getVaoId() const
{
    return vao_id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the OpenGL ID of the Index Buffer Object for this mesh.
///
/// \return The ID for this mesh's IBO.
GLuint Mesh::getIboId() const
{
    return ibo_id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the number of indices in the mesh's IBO.
///
/// \return the size of the IBO.
GLsizei Mesh::getIndexCount() const
{
    return index_count_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Binds this mesh's VAO and draws the mesh.
/// 
/// \note   Batcher and BatcherTask should probably be used instead to avoid
///         binding and unbinding the same VAO multiple times in a row.
void Mesh::drawElements()
{
    // note: does not bind a shader program or set any uniforms.

    glBindVertexArray(vao_id_); // bind VAO

    glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);       // unbind VAO
}

} // namespace pbj::gfx
} // namespace pbj
