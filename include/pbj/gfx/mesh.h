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

struct MeshVertexAttributeSpec
{
    GLint indices[16];

    MeshVertexAttributeSpec();
};

class Mesh
{
public:
    Mesh(const sw::ResourceId& id,
        GLvoid* vertex_data,
        const MeshVertexAttributeSpec& attributes,
        GLsizei n_vertices,
        GLvoid* index_data,
        GLsizei n_indices,
        GLenum index_data_type = GL_UNSIGNED_SHORT);
    ~Mesh();
    
    const be::Handle<Mesh>& getHandle();
    const be::ConstHandle<Mesh>& getHandle() const;

    const sw::ResourceId& getId() const;

    GLuint getVaoId() const;
    GLuint getIboId() const;
    GLsizei getIndexCount() const;
    GLenum getIndexType() const;

    void drawElements();

private:
    be::SourceHandle<Mesh> handle_;
    sw::ResourceId resource_id_;

    GLuint vao_id_; ///< OpenGL Vertex array object id
    GLuint ibo_id_; ///< OpenGL Vertex Index buffer object id
    GLuint vbo_id_; ///< OpenGL Vertex buffer object id
    GLsizei index_count_; ///< Size of IBO
    GLenum ibo_data_type_; ///< Data type of IBO elements.
    
    

    Mesh(const Mesh&);
    void operator=(const Mesh&);
};

} // namespace pbj::gfx
} // namespace pbj

#endif
