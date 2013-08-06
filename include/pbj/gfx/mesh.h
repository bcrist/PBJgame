///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/mesh.h
/// \author Benjamin Crist
///
/// \brief  pbj::gfx::Mesh class header.

#ifndef PBJ_GFX_MESH_H_
#define PBJ_GFX_MESH_H_

#include "pbj/_gl.h"
#include "pbj/_pbj.h"
#include "pbj/_math.h"

#include "pbj/sw/resource_id.h"
#include "be/source_handle.h"

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
/// \brief  A MeshVertexAttributeSpec describes the OpenGL vertex attribute
///         indices that correspond to each vec4 in vertex_data.
///
/// \details The number of vec4's that correspond to a single vertex is
///         determined by the first negative index in indices.  For instance
///         
struct MeshVertexAttributeSpec
{
    GLint indices[16];

    MeshVertexAttributeSpec();
};

///////////////////////////////////////////////////////////////////////////////
/// \brief  Represents a set of vertex data, including positions and all other
///         associated data required to render the vertex. (eg. texcoords,
///         vertex colors, etc.
class Mesh
{
public:
    Mesh(const sw::ResourceId& id,
        vec4* vertex_data,
        const MeshVertexAttributeSpec& attributes,
        GLsizei n_vertices,
        GLushort* index_data,
        GLsizei n_indices);
    ~Mesh();
    
    const be::Handle<Mesh>& getHandle();
    const be::ConstHandle<Mesh>& getHandle() const;

    const sw::ResourceId& getId() const;

    GLuint getVaoId() const;
    GLuint getIboId() const;
    GLsizei getIndexCount() const;

    void drawElements();

private:
    be::SourceHandle<Mesh> handle_;
    sw::ResourceId resource_id_;

    GLuint vao_id_; ///< OpenGL Vertex array object id
    GLuint ibo_id_; ///< OpenGL Vertex Index buffer object id
    GLuint vbo_id_; ///< OpenGL Vertex buffer object id
    GLsizei index_count_; ///< Size of IBO

    Mesh(const Mesh&);
    void operator=(const Mesh&);
};

} // namespace pbj::gfx
} // namespace pbj

#endif
