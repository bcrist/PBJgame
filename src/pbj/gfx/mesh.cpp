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
/// \file   pbj/gfx/mesh.h
/// \author Benjamin Crist
///
/// \brief  pbj::gfx::Mesh class header.

#include "pbj/gfx/mesh.h"

#include <iostream>

namespace pbj {
namespace gfx {

MeshVertexAttributeSpec::MeshVertexAttributeSpec()
{
    for (int i = 0; i < 16; ++i)
        indices[i] = -1;
}

Mesh::Mesh(const sw::ResourceId& id,
           GLvoid* vertex_data,
           const MeshVertexAttributeSpec& attributes,
           GLsizei n_vertices,
           GLvoid* index_data,
           GLsizei n_indices,
           GLenum index_data_type)
    : resource_id_(id),
      index_count_(n_indices),
      ibo_data_type_(index_data_type)
{
    handle_.associate(this);

    GLsizei index_data_size = n_indices;
    switch (index_data_type)
    {
        case GL_UNSIGNED_BYTE:
            break;

        case GL_UNSIGNED_SHORT:
            index_data_size *= 2;
            break;

        case GL_UNSIGNED_INT:
            index_data_size *= 4;
            break;

        default:
            PBJ_LOG(VWarning) << "Unrecognized index data type!  Using GL_UNSIGNED_SHORT instead." << PBJ_LOG_NL
                              << "Sandwich ID: " << id.sandwich << PBJ_LOG_NL
                              << "Mesh ID: " << id.resource << PBJ_LOG_NL
                              << "Data Type Code: " << index_data_type << PBJ_LOG_END;
            index_data_type = ibo_data_type_ = GL_UNSIGNED_SHORT;
            index_data_size *= 2;
            break;
    }

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

    // Generate & fill VBO
    glGenBuffers(1, &vbo_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_); // bind VBO
    glBufferData(GL_ARRAY_BUFFER, stride * n_vertices, vertex_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);       // unbind VBO

    // Generate & fill IBO
    glGenBuffers(1, &ibo_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id_); // bind IBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data_size, index_data, GL_STATIC_DRAW);
    // don't unbind IBO because it is part of the VAO's state.

    // set up vertex attribute pointers & enable them
    for (size_t i = 0; i < n_attributes; ++i)
    {
        GLuint attribute_index = attributes.indices[i];
        glVertexAttribPointer(attribute_index, 4, GL_FLOAT, false, stride, reinterpret_cast<void*>(i * sizeof(vec4)));
        glEnableVertexAttribArray(attribute_index);
    }

    glBindVertexArray(0);                   // unbind VAO
}
Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &vao_id_);
    glDeleteBuffers(1, &vbo_id_);
    glDeleteBuffers(1, &ibo_id_);
}

    
const be::Handle<Mesh>& Mesh::getHandle()
{
    return handle_;
}

const be::ConstHandle<Mesh>& Mesh::getHandle() const
{
    return handle_;
}

const sw::ResourceId& Mesh::getId() const
{
    return resource_id_;
}

GLuint Mesh::getVaoId() const
{
    return vao_id_;
}

GLuint Mesh::getIboId() const
{
    return ibo_id_;
}

GLsizei Mesh::getIndexCount() const
{
    return index_count_;
}

GLenum Mesh::getIndexType() const
{
    return ibo_data_type_;
}

void Mesh::drawElements()
{
    // note: does not bind a shader program or set any uniforms.

    glBindVertexArray(vao_id_); // bind VAO

    glDrawElements(GL_TRIANGLES, index_count_, ibo_data_type_, 0);

    glBindVertexArray(0);       // unbind VAO
}

} // namespace pbj::gfx
} // namespace pbj
