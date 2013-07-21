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
/// \file   pbj/scene/shader.h
/// \author Benjamin Crist
///
/// \brief  pbj::scene::Shader class header.

#ifndef PBJ_GFX_SHADER_H_
#define PBJ_GFX_SHADER_H_

#include <map>

#include "pbj/_pbj.h"
#include "pbj/_gl.h"
#include "be/source_handle.h"
#include "pbj/sw/resource_id.h"

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Represents a compiled Vertex/Fragment Shader
///
/// \sa     ShaderProgram
class Shader
{
public:
    enum Type
    {
        TVertex,
        TFragment
    };

    Shader(const sw::ResourceId& id, Type type, const std::string& source);
    ~Shader();

    be::Handle<Shader> getHandle();
    const be::ConstHandle<Shader> getHandle() const;

    const sw::ResourceId& getId() const;

    Type getType() const;

    GLuint getGlId() const;

#ifdef PBJ_EDITOR
    Shader();   // construct without compiling

    void setName(const std::string& name);
    const std::string& getName() const;

    void setSandwich(const Id& id);

    void setMetadata(const std::string& key, const std::string& value);
    const std::string& getMetadata(const std::string& key) const;
    const std::map<std::string, std::string>& getMetadata() const;

    void setSource(const std::string& source);
    const std::string& getSource() const;

    void setType(Type type);

    bool isValid() const;
    const std::string& getInfoLog() const;

    void compile();
#endif

private:
    void compile_(const std::string& source);
    void invalidate_();

    be::SourceHandle<Shader> handle_;

    sw::ResourceId resource_id_;

    GLuint gl_id_;
    Type type_;

#ifdef PBJ_EDITOR
    std::string& nullString_() const;

    std::map<std::string, std::string> metadata_;
    // special metadata keys:
    // __name__ => the name of the asset
    // __source__ => the shader's source code
    // __infolog__ => the result of the last attempt to compile the shader.
#endif

    Shader(const Shader&);
    void operator=(const Shader&);
};

} // namespace pbj::gfx
} // namespace pbj

#endif
