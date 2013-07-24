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
/// \file   pbj/gfx/mesh_instance.h
/// \author Benjamin Crist
///
/// \brief  pbj::gfx::MeshInstance class header.

#ifndef PBJ_GFX_MESH_INSTANCE_H_
#define PBJ_GFX_MESH_INSTANCE_H_

#include "pbj/gfx/mesh.h"
#include "pbj/gfx/shader_program.h"

namespace pbj {
namespace gfx {

class MeshInstance
{
public:
    MeshInstance(const be::ConstHandle<Mesh>& mesh, const be::ConstHandle<ShaderProgram>& program);
    ~MeshInstance();
    
    void setMesh(const be::ConstHandle<Mesh>& mesh);
    const be::ConstHandle<Mesh>& getMesh() const;

    void setProgram(const be::ConstHandle<ShaderProgram>& program);
    const be::ConstHandle<ShaderProgram>& getProgram() const;

    void draw(const mat4& projection, const mat4& view, const mat4& model);

private:
    be::ConstHandle<Mesh> mesh_;
    be::ConstHandle<ShaderProgram> program_;
    
    GLint mvp_location_;
    GLint p_location_;
    GLint v_location_;
    GLint m_location_;

    MeshInstance(const MeshInstance&);
    void operator=(const MeshInstance&);
};

} // namespace pbj::gfx
} // namespace pbj

#endif
