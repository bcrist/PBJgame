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
/// \file   pbj/gfx/built_ins.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::gfx::BuiltIns functions.

#include "pbj/gfx/built_ins.h"

#include "pbj/gfx/mesh.h"
#include "pbj/gfx/texture.h"
#include "pbj/gfx/shader.h"
#include "pbj/gfx/shader_program.h"

#include <iostream>

namespace pbj {
namespace gfx {

//class Mesh;
class Texture;
class Shader;
class ShaderProgram;


/*
const Mesh& BuiltIns::getMesh(const Id& id) const
{
    auto i = meshes_.find(id);
    if (i != meshes_.end())
        return *i->second;

    // TODO: log warning
    throw std::invalid_argument("Mesh not found!");
}*/
    
const Texture& BuiltIns::getTexture(const Id& id) const
{
    auto i = textures_.find(id);
    if (i != textures_.end())
        return *i->second;

    // TODO: log warning
    throw std::invalid_argument("Texture not found!");
}

const Shader& BuiltIns::getShader(const Id& id) const
{
    auto i = shaders_.find(id);
    if (i != shaders_.end())
        return *i->second;

    // TODO: log warning
    throw std::invalid_argument("Shader not found!");
}

const ShaderProgram& BuiltIns::getProgram(const Id& id) const
{
    auto i = programs_.find(id);
    if (i != programs_.end())
        return *i->second;

    // TODO: log warning
    throw std::invalid_argument("Program not found!");
}

BuiltIns::BuiltIns()
{
    sw::ResourceId id(Id(0), Id(0));

    id.resource = Id("Shader.TextureFontText.vertex");
    try
    {
        Shader* shader = new Shader(id, Shader::TVertex,
            "#version 330\n\n"
            "uniform mat4 transform;\n\n"
            "layout(location = 0) in vec2 in_position;\n"
            "layout(location = 1) in vec2 in_texcoord;\n\n"
            "out vec2 texcoord;\n\n"
            "void main()\n"
            "{\n"
            "   texcoord = in_texcoord;\n"
            "   gl_Position = transform * vec4(in_position, 0.0, 1.0);\n"
            "}\n");
        shaders_.insert(std::make_pair(shader->getId().resource, std::unique_ptr<Shader>(shader)));
    }
    catch (const std::exception& err)
    {
        logWarning("Shader", id, err.what());
    }

    id.resource = Id("Shader.TextureFontText.fragment");
    try
    {
        Shader* shader = new Shader(id, Shader::TFragment,
            "#version 330\n\n"
            "uniform vec4 color;\n"
            "uniform sampler2D texture;\n\n"
            "in vec2 texcoord;\n\n"
            "layout(location = 0) out vec4 out_fragcolor;\n\n"
            "void main()\n"
            "{\n"
            "   vec4 fragcolor = color;\n"
            "   fragcolor.a *= texture(texture, texcoord).r;\n"
            "   out_fragcolor = fragcolor;\n"
            "}\n");
        shaders_.insert(std::make_pair(shader->getId().resource, std::unique_ptr<Shader>(shader)));
    }
    catch (const std::exception& err)
    {
        logWarning("Shader", id, err.what());
    }

    id.resource = Id("ShaderProgram.TextureFontText");
    try
    {
        ShaderProgram* program = new ShaderProgram(id,
            getShader(Id("Shader.TextureFontText.vertex")), 
            getShader(Id("Shader.TextureFontText.fragment")));
        programs_.insert(std::make_pair(program->getId().resource, std::unique_ptr<ShaderProgram>(program)));
    }
    catch (const std::exception& err)
    {
        logWarning("Program", id, err.what());
    }
}

BuiltIns::~BuiltIns()
{
}

void BuiltIns::logWarning(const char* type, const sw::ResourceId id, const std::string& what_arg) const
{
    PBJ_LOG(VWarning) << "Exception while initializing built-in resource!" << PBJ_LOG_NL
                      << "Resource Type: " << type << PBJ_LOG_NL
                      << "  Resource ID: " << id.resource << PBJ_LOG_NL
                      << "    Exception: " << what_arg << PBJ_LOG_END;
}


} // namespace pbj::gfx
} // namespace pbj
