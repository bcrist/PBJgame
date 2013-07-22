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
/// \file   pbj/gfx/built_ins.h
/// \author Benjamin Crist
///
/// \brief  pbj::gfx::BuiltIns class header.

#ifndef PBJ_GFX_BUILT_INS_H_
#define PBJ_GFX_BUILT_INS_H_

#include "pbj/sw/resource_id.h"
#include "pbj/_pbj.h"

#include <unordered_map>
#include <memory>

namespace pbj {

class Engine;

namespace gfx {

class Mesh;
class Texture;
class Shader;
class ShaderProgram;

class BuiltIns
{
    friend class Engine;
    friend struct std::default_delete<BuiltIns>;

public:
    //const Mesh& getMesh(const Id& id) const;
    
    const Texture& getTexture(const Id& id) const;

    const Shader& getShader(const Id& id) const;
    const ShaderProgram& getProgram(const Id& id) const;

private:
    BuiltIns();
    ~BuiltIns();

    void logWarning(const char* type, const sw::ResourceId id, const std::string& what_arg) const;

    //std::unordered_map<Id, std::unique_ptr<Mesh> > meshes_;

    std::unordered_map<Id, std::unique_ptr<Texture> > textures_;

    std::unordered_map<Id, std::unique_ptr<Shader> > shaders_;
    std::unordered_map<Id, std::unique_ptr<ShaderProgram> > programs_;


    BuiltIns(const BuiltIns&);
    void operator=(const BuiltIns&);
};

} // namespace pbj::gfx
} // namespace pbj

#endif
