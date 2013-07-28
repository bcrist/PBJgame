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


#include "pbj/gfx/texture_font.h"
#include "pbj/gfx/mesh.h"
#include "pbj/gfx/texture.h"
#include "pbj/gfx/shader.h"
#include "pbj/gfx/shader_program.h"

#include "pbj/sw/import.h"

#include "pbj/_gl.h"

#include <iostream>

namespace pbj {
namespace gfx {

const TextureFont& BuiltIns::getTextureFont(const Id& id) const
{
    auto i = texture_fonts_.find(id);
    if (i != texture_fonts_.end())
        return *i->second;

    // TODO: log warning
    throw std::invalid_argument("TextureFont not found!");
}


const Mesh& BuiltIns::getMesh(const Id& id) const
{
    auto i = meshes_.find(id);
    if (i != meshes_.end())
        return *i->second;

    // TODO: log warning
    throw std::invalid_argument("Mesh not found!");
}
    
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
    GLenum err;
    sw::ResourceId id(Id(0), Id(0));

        while ((err = glGetError()) != GL_NO_ERROR)
    {
        PBJ_LOG(pbj::VWarning) << "OpenGL error!" << PBJ_LOG_NL
                               << "Error Code: " << err << PBJ_LOG_NL
                               << "     Error: " << pbj::getGlErrorString(err) << PBJ_LOG_END;
    }

    id.resource = Id("Mesh.std_quad");
    try
    {
        MeshVertexAttributeSpec attr_spec;
        attr_spec.indices[0] = 0; // vertex position
        attr_spec.indices[1] = 1; // vertex normal
        attr_spec.indices[2] = 2; // vertex texcoords
        

        std::vector<vec4> verts;
        verts.push_back(vec4(0, 0, 0, 1));
        verts.push_back(vec4(0, 0, 1, 0));
        verts.push_back(vec4(0, 0, 0, 1));

        verts.push_back(vec4(1, 0, 0, 1));
        verts.push_back(vec4(0, 0, 1, 0));
        verts.push_back(vec4(1, 0, 0, 1));

        verts.push_back(vec4(1, 1, 0, 1));
        verts.push_back(vec4(0, 0, 1, 0));
        verts.push_back(vec4(1, 1, 0, 1));
        
        verts.push_back(vec4(0, 1, 0, 1));
        verts.push_back(vec4(0, 0, 1, 0));
        verts.push_back(vec4(0, 1, 0, 1));

        std::vector<U8> indices;
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);

        indices.push_back(0);
        indices.push_back(2);
        indices.push_back(3);

        Mesh* mesh = new Mesh(id, verts.data(), attr_spec, 4, indices.data(), indices.size(), GL_UNSIGNED_BYTE);
        meshes_.insert(std::make_pair(id.resource, std::unique_ptr<Mesh>(mesh)));
    }
    catch (const std::exception& err)
    {
        logWarning("Mesh", id, err.what());
    }

        while ((err = glGetError()) != GL_NO_ERROR)
    {
        PBJ_LOG(pbj::VWarning) << "OpenGL error!" << PBJ_LOG_NL
                               << "Error Code: " << err << PBJ_LOG_NL
                               << "     Error: " << pbj::getGlErrorString(err) << PBJ_LOG_END;
    }

    id.resource = Id("Shader.UIButton.vertex");
    try
    {
        Shader* shader = new Shader(id, Shader::TVertex,
            "#version 330\n\n"
            "uniform mat4 transform;\n\n"
            "layout(location = 0) in vec4 in_position;\n\n"
            "out vec2 position;\n\n"
            "void main()\n"
            "{\n"
            "   position = in_position.xy;\n"
            "   gl_Position = transform * in_position;\n"
            "}\n");
        shaders_.insert(std::make_pair(shader->getId().resource, std::unique_ptr<Shader>(shader)));
    }
    catch (const std::exception& err)
    {
        logWarning("Shader", id, err.what());
    }

        while ((err = glGetError()) != GL_NO_ERROR)
    {
        PBJ_LOG(pbj::VWarning) << "OpenGL error!" << PBJ_LOG_NL
                               << "Error Code: " << err << PBJ_LOG_NL
                               << "     Error: " << pbj::getGlErrorString(err) << PBJ_LOG_END;
    }

    id.resource = Id("Shader.UIButton.fragment");
    try
    {
        Shader* shader = new Shader(id, Shader::TFragment,
            "#version 330\n\n"
            "uniform vec2 border_bounds[4];\n"
            "uniform vec4 border_color;\n"
            "uniform vec4 background_color;\n\n"
            "uniform vec4 outside_color;\n\n"
            "in vec2 position;\n\n"
            "layout(location = 0) out vec4 out_fragcolor;\n\n"
            "void main()\n"
            "{\n"
            "   if (position.x >= border_bounds[0].x && position.x <= border_bounds[1].x &&\n"
            "       position.y >= border_bounds[0].y && position.y <= border_bounds[1].y)\n"
            "   {\n"
            "      out_fragcolor = background_color;\n"
            "   }\n"
            "   else if (position.x >= border_bounds[2].x && position.x <= border_bounds[3].x &&\n"
            "            position.y >= border_bounds[2].y && position.y <= border_bounds[3].y)\n"
            "   {\n"
            "      out_fragcolor = border_color;\n"
            "   }\n"
            "   else\n"
            "   {\n"
            "      out_fragcolor = outside_color;\n"
            "   }\n"
            "}\n");
        shaders_.insert(std::make_pair(shader->getId().resource, std::unique_ptr<Shader>(shader)));
    }
    catch (const std::exception& err)
    {
        logWarning("Shader", id, err.what());
    }

        while ((err = glGetError()) != GL_NO_ERROR)
    {
        PBJ_LOG(pbj::VWarning) << "OpenGL error!" << PBJ_LOG_NL
                               << "Error Code: " << err << PBJ_LOG_NL
                               << "     Error: " << pbj::getGlErrorString(err) << PBJ_LOG_END;
    }

    id.resource = Id("ShaderProgram.UIButton");
    try
    {
        ShaderProgram* program = new ShaderProgram(id,
            getShader(Id("Shader.UIButton.vertex")), 
            getShader(Id("Shader.UIButton.fragment")));
        programs_.insert(std::make_pair(program->getId().resource, std::unique_ptr<ShaderProgram>(program)));
    }
    catch (const std::exception& err)
    {
        logWarning("ShaderProgram", id, err.what());
    }

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
            "uniform sampler2D texsampler;\n\n"
            "in vec2 texcoord;\n\n"
            "layout(location = 0) out vec4 out_fragcolor;\n\n"
            "void main()\n"
            "{\n"
            "   out_fragcolor = vec4(color.rgb, color.a * texture(texsampler, texcoord).r);\n"
            //"   out_fragcolor = color;\n"
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
        logWarning("ShaderProgram", id, err.what());
    }

    id.resource = Id("Texture.TextureFont.default");
    try
    {
        const GLubyte* data = reinterpret_cast<const GLubyte*>(
            "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A\x00\x00\x00\x0D\x49\x48\x44\x52"
            "\x00\x00\x00\x80\x00\x00\x00\x80\x08\x03\x00\x00\x00\xF4\xE0\x91"
            "\xF9\x00\x00\x00\x19\x74\x45\x58\x74\x53\x6F\x66\x74\x77\x61\x72"
            "\x65\x00\x41\x64\x6F\x62\x65\x20\x49\x6D\x61\x67\x65\x52\x65\x61"
            "\x64\x79\x71\xC9\x65\x3C\x00\x00\x03\x66\x69\x54\x58\x74\x58\x4D"
            "\x4C\x3A\x63\x6F\x6D\x2E\x61\x64\x6F\x62\x65\x2E\x78\x6D\x70\x00"
            "\x00\x00\x00\x00\x3C\x3F\x78\x70\x61\x63\x6B\x65\x74\x20\x62\x65"
            "\x67\x69\x6E\x3D\x22\xEF\xBB\xBF\x22\x20\x69\x64\x3D\x22\x57\x35"
            "\x4D\x30\x4D\x70\x43\x65\x68\x69\x48\x7A\x72\x65\x53\x7A\x4E\x54"
            "\x63\x7A\x6B\x63\x39\x64\x22\x3F\x3E\x20\x3C\x78\x3A\x78\x6D\x70"
            "\x6D\x65\x74\x61\x20\x78\x6D\x6C\x6E\x73\x3A\x78\x3D\x22\x61\x64"
            "\x6F\x62\x65\x3A\x6E\x73\x3A\x6D\x65\x74\x61\x2F\x22\x20\x78\x3A"
            "\x78\x6D\x70\x74\x6B\x3D\x22\x41\x64\x6F\x62\x65\x20\x58\x4D\x50"
            "\x20\x43\x6F\x72\x65\x20\x35\x2E\x30\x2D\x63\x30\x36\x31\x20\x36"
            "\x34\x2E\x31\x34\x30\x39\x34\x39\x2C\x20\x32\x30\x31\x30\x2F\x31"
            "\x32\x2F\x30\x37\x2D\x31\x30\x3A\x35\x37\x3A\x30\x31\x20\x20\x20"
            "\x20\x20\x20\x20\x20\x22\x3E\x20\x3C\x72\x64\x66\x3A\x52\x44\x46"
            "\x20\x78\x6D\x6C\x6E\x73\x3A\x72\x64\x66\x3D\x22\x68\x74\x74\x70"
            "\x3A\x2F\x2F\x77\x77\x77\x2E\x77\x33\x2E\x6F\x72\x67\x2F\x31\x39"
            "\x39\x39\x2F\x30\x32\x2F\x32\x32\x2D\x72\x64\x66\x2D\x73\x79\x6E"
            "\x74\x61\x78\x2D\x6E\x73\x23\x22\x3E\x20\x3C\x72\x64\x66\x3A\x44"
            "\x65\x73\x63\x72\x69\x70\x74\x69\x6F\x6E\x20\x72\x64\x66\x3A\x61"
            "\x62\x6F\x75\x74\x3D\x22\x22\x20\x78\x6D\x6C\x6E\x73\x3A\x78\x6D"
            "\x70\x4D\x4D\x3D\x22\x68\x74\x74\x70\x3A\x2F\x2F\x6E\x73\x2E\x61"
            "\x64\x6F\x62\x65\x2E\x63\x6F\x6D\x2F\x78\x61\x70\x2F\x31\x2E\x30"
            "\x2F\x6D\x6D\x2F\x22\x20\x78\x6D\x6C\x6E\x73\x3A\x73\x74\x52\x65"
            "\x66\x3D\x22\x68\x74\x74\x70\x3A\x2F\x2F\x6E\x73\x2E\x61\x64\x6F"
            "\x62\x65\x2E\x63\x6F\x6D\x2F\x78\x61\x70\x2F\x31\x2E\x30\x2F\x73"
            "\x54\x79\x70\x65\x2F\x52\x65\x73\x6F\x75\x72\x63\x65\x52\x65\x66"
            "\x23\x22\x20\x78\x6D\x6C\x6E\x73\x3A\x78\x6D\x70\x3D\x22\x68\x74"
            "\x74\x70\x3A\x2F\x2F\x6E\x73\x2E\x61\x64\x6F\x62\x65\x2E\x63\x6F"
            "\x6D\x2F\x78\x61\x70\x2F\x31\x2E\x30\x2F\x22\x20\x78\x6D\x70\x4D"
            "\x4D\x3A\x4F\x72\x69\x67\x69\x6E\x61\x6C\x44\x6F\x63\x75\x6D\x65"
            "\x6E\x74\x49\x44\x3D\x22\x78\x6D\x70\x2E\x64\x69\x64\x3A\x37\x37"
            "\x31\x38\x44\x42\x46\x34\x45\x45\x45\x38\x45\x32\x31\x31\x41\x36"
            "\x38\x45\x44\x37\x33\x37\x34\x30\x32\x36\x39\x36\x38\x30\x22\x20"
            "\x78\x6D\x70\x4D\x4D\x3A\x44\x6F\x63\x75\x6D\x65\x6E\x74\x49\x44"
            "\x3D\x22\x78\x6D\x70\x2E\x64\x69\x64\x3A\x31\x34\x32\x36\x31\x43"
            "\x45\x37\x45\x38\x45\x46\x31\x31\x45\x32\x38\x45\x36\x35\x38\x30"
            "\x35\x38\x42\x35\x37\x35\x33\x30\x43\x38\x22\x20\x78\x6D\x70\x4D"
            "\x4D\x3A\x49\x6E\x73\x74\x61\x6E\x63\x65\x49\x44\x3D\x22\x78\x6D"
            "\x70\x2E\x69\x69\x64\x3A\x31\x34\x32\x36\x31\x43\x45\x36\x45\x38"
            "\x45\x46\x31\x31\x45\x32\x38\x45\x36\x35\x38\x30\x35\x38\x42\x35"
            "\x37\x35\x33\x30\x43\x38\x22\x20\x78\x6D\x70\x3A\x43\x72\x65\x61"
            "\x74\x6F\x72\x54\x6F\x6F\x6C\x3D\x22\x41\x64\x6F\x62\x65\x20\x50"
            "\x68\x6F\x74\x6F\x73\x68\x6F\x70\x20\x43\x53\x35\x2E\x31\x20\x57"
            "\x69\x6E\x64\x6F\x77\x73\x22\x3E\x20\x3C\x78\x6D\x70\x4D\x4D\x3A"
            "\x44\x65\x72\x69\x76\x65\x64\x46\x72\x6F\x6D\x20\x73\x74\x52\x65"
            "\x66\x3A\x69\x6E\x73\x74\x61\x6E\x63\x65\x49\x44\x3D\x22\x78\x6D"
            "\x70\x2E\x69\x69\x64\x3A\x37\x37\x31\x38\x44\x42\x46\x34\x45\x45"
            "\x45\x38\x45\x32\x31\x31\x41\x36\x38\x45\x44\x37\x33\x37\x34\x30"
            "\x32\x36\x39\x36\x38\x30\x22\x20\x73\x74\x52\x65\x66\x3A\x64\x6F"
            "\x63\x75\x6D\x65\x6E\x74\x49\x44\x3D\x22\x78\x6D\x70\x2E\x64\x69"
            "\x64\x3A\x37\x37\x31\x38\x44\x42\x46\x34\x45\x45\x45\x38\x45\x32"
            "\x31\x31\x41\x36\x38\x45\x44\x37\x33\x37\x34\x30\x32\x36\x39\x36"
            "\x38\x30\x22\x2F\x3E\x20\x3C\x2F\x72\x64\x66\x3A\x44\x65\x73\x63"
            "\x72\x69\x70\x74\x69\x6F\x6E\x3E\x20\x3C\x2F\x72\x64\x66\x3A\x52"
            "\x44\x46\x3E\x20\x3C\x2F\x78\x3A\x78\x6D\x70\x6D\x65\x74\x61\x3E"
            "\x20\x3C\x3F\x78\x70\x61\x63\x6B\x65\x74\x20\x65\x6E\x64\x3D\x22"
            "\x72\x22\x3F\x3E\x5B\x0F\xB4\x12\x00\x00\x00\x06\x50\x4C\x54\x45"
            "\x00\x00\x00\xFF\xFF\xFF\xA5\xD9\x9F\xDD\x00\x00\x05\x98\x49\x44"
            "\x41\x54\x78\xDA\xEC\x5A\xD1\x92\x9C\x30\x0C\xB3\xFE\xFF\xA7\xDB"
            "\x99\xDB\x05\x49\x96\x43\xD8\xF6\xAE\x7D\x20\x9D\xC2\x2E\xCB\x05"
            "\xC7\xB1\x65\x5B\xA6\xAA\x0A\xBF\xFF\x9D\xA7\xD7\xB1\x7F\xC0\x71"
            "\x01\x76\x3D\x5E\x6E\xBF\x03\x28\x9E\x08\xE7\xFD\x88\x13\x4C\xD7"
            "\xA7\x33\x5C\xD0\x2E\x27\x2F\x14\x24\xE9\xCF\x8C\x43\x03\x76\xF9"
            "\x2D\xF5\xD7\xA8\x7A\x9F\xDE\xBF\xBE\xBE\xFB\x75\x3F\x17\xFF\x39"
            "\xD0\xCE\x3E\x0F\x9D\x45\x8B\xE3\x03\xFA\x03\x57\xF7\xD7\x68\x2F"
            "\x61\x1E\xF4\x6D\x1A\x1F\xAC\x77\xCE\xF7\xDF\x12\xE0\x58\x07\xD6"
            "\x37\xDA\xD6\x4C\xF7\xCB\x56\xF8\x39\x6D\x4D\x75\x2D\x44\x01\xFA"
            "\xF7\x85\xC0\xF7\x34\xF0\x65\x08\x58\xDF\x88\x6F\x15\xA0\xB6\xAC"
            "\x55\xCF\xC7\x9F\x9A\x97\x0C\xAA\x5E\x6B\xF6\x5F\x8F\x04\xA9\x62"
            "\x1E\xEE\x26\x01\xD5\xF4\xFF\x5B\x2B\xFC\x33\xDE\xB6\x86\xD9\xC2"
            "\xEC\xF9\x87\x71\x9E\x6A\xFF\x74\x41\xB0\xCF\xB8\x02\x63\xD8\x17"
            "\x6C\xA1\xA2\xEC\x37\x3F\x05\xF2\x55\xAF\x87\x49\xFD\x84\xC1\xE2"
            "\x33\x1A\xD6\x85\x00\xC7\xE6\x01\x13\x94\x76\x39\xF2\x8A\x33\x1A"
            "\xBA\x00\x30\xB7\x35\x0D\x6C\x09\x40\xAA\x13\x97\x75\xCD\x04\x41"
            "\x67\x0D\xF0\xB6\x9A\xBF\xE6\x2D\xD8\x05\x23\xDB\xD6\x2B\x1B\x88"
            "\x98\xBF\xBD\x05\x83\x00\xBA\x0D\xA8\xF1\x7F\x9E\x74\xD8\x82\x1A"
            "\x03\x92\xA3\xE0\x91\x6E\xD1\x3E\x89\xD5\xB3\x67\x26\x0D\x38\x16"
            "\xFD\x27\x70\x79\xD8\x1D\x85\xAE\xC3\xA8\x1D\xCF\xC4\x0E\x11\x7F"
            "\x65\xA3\xE6\x0C\xB5\xF2\x27\xDB\x1C\x4E\x44\x43\x92\x0C\x75\x41"
            "\x84\x5F\xE1\x50\x09\x54\x4C\x8D\x92\xA1\xA1\x18\x9B\xAF\x70\xFA"
            "\x02\xC5\xD1\xB1\x19\xE9\x53\xC6\x09\xA8\xAB\x2C\x8F\x25\x47\x8A"
            "\xEA\xA4\x01\xB6\x7F\xCC\xEE\xAA\x1A\xFE\x93\xA3\x69\x00\x87\xC5"
            "\x08\xFA\x00\xC3\x16\xA0\x7F\x9E\x8E\x27\xDA\x95\x20\x9E\x6A\x00"
            "\x4D\x3E\x09\xE3\xAF\x5B\x74\x5F\x52\x68\xC8\xC7\x8A\xB9\x85\x45"
            "\x8B\x23\x93\x28\xC7\xC6\x58\x76\xB1\x4B\xDE\xB5\x81\x2C\x40\x14"
            "\xE5\x5B\xB1\xE8\x6B\x8B\xB7\xF2\x1D\x0F\x02\x98\x52\x03\x1C\xC0"
            "\xC5\x08\x6D\xC6\x96\x17\x86\x01\x5E\x90\x72\x36\x7A\x20\xF4\x90"
            "\xA1\x84\x05\xA0\x02\x3D\x65\x0F\x88\xC0\xD6\x25\xEA\x8B\xC0\x02"
            "\xCB\xCC\xB2\xA3\x69\xEE\x04\x83\x73\x81\xD0\xE5\xC2\xCC\x93\x93"
            "\x3C\xB1\x3B\xF3\xED\x56\xC2\x0F\x6E\xED\xC0\x8C\x6A\xCB\xA2\x8C"
            "\xE3\x4A\x03\xC5\xC6\x32\x3F\xD4\x41\x41\x0F\xE8\xB3\x42\xA0\x63"
            "\x47\x80\x1B\xA8\x89\x66\x01\xAD\x84\x81\xD8\xB0\x7B\x41\xBD\x70"
            "\xB0\x3E\x13\x80\x72\x7E\x30\xE5\xE2\x21\xFA\x83\xB2\x0C\x12\xDC"
            "\x4A\x12\x4A\x8D\x5C\xDB\x99\x0B\x3A\x69\x13\xCB\x58\xB5\x81\x5D"
            "\x37\x98\xEB\x65\x60\x22\xF1\x46\x6E\x64\x51\x96\x25\x50\x5E\x32"
            "\x06\x3A\x29\x06\x7C\x40\x4B\x4B\xCF\x5C\x8F\x9F\x44\x11\xB9\xAA"
            "\x16\x2B\xF3\xF4\xDB\xC9\x99\x2A\xAE\x99\x31\x06\x1F\x8E\x7E\x2A"
            "\x35\x18\xDD\x03\x81\x47\x30\xC3\x08\x99\x61\xAE\x69\xE0\xC4\x24"
            "\xCA\x51\x2D\x34\x78\x1A\x5C\xC1\xE8\x67\x2E\x64\xE2\xDF\x24\xE5"
            "\x79\xE7\x19\x6A\x19\x10\x1F\xBB\x26\xEE\x2A\xD6\xDB\x4B\x32\xE7"
            "\x08\xC7\x9E\x78\x7B\xC4\xC5\xCC\x06\x95\xEF\xF5\x47\xFC\x8C\x67"
            "\x40\xB0\x07\xDF\x9E\x39\x53\xB8\x6C\xEA\xEC\xBC\xEA\x05\xF7\x60"
            "\x66\xF2\x76\x6C\xD3\x7D\x6B\xD8\x69\x4C\xE7\xC0\xFB\x69\x2B\xA0"
            "\x67\xA3\xFD\x7C\xF8\x42\xDA\xD9\xFB\x04\x64\xAE\x72\x16\x14\x6E"
            "\x73\xBE\x0B\x1B\x87\x6C\x01\x15\xDA\x86\x37\x90\x92\x3C\xCF\x27"
            "\xF9\xFE\xB6\x00\xA2\x99\xF5\x16\x80\x32\x86\x38\x9F\x26\x6E\x89"
            "\x62\xAD\x59\x03\xB5\xC1\x29\x07\xBC\xC4\xE0\x1D\xDB\xE4\x74\xEF"
            "\x48\x65\x60\xBE\xCD\x01\xEF\xB5\x60\xCA\xF6\x7A\x7B\x72\x68\x92"
            "\x0D\xA9\x76\x23\xEF\x73\xFD\x1C\x61\xC0\xCA\x14\x2C\xD9\x05\x57"
            "\x19\x6D\xD6\xAB\xAE\xD9\x44\xC3\x1A\x49\x8D\x5E\x31\x36\x43\x24"
            "\xDE\x4A\x33\x60\xAE\xA0\x46\x1A\xD6\xA9\x66\x84\x9A\x31\x09\x20"
            "\xF5\x74\xC7\xA6\xB9\xF8\x4A\x75\x77\xB8\xD1\xB2\x73\x13\x80\xD3"
            "\x55\x2E\x0B\x8D\x2B\xE8\x73\x05\x42\xA9\xA5\x68\xAD\x3E\xEA\x52"
            "\xC1\xC2\x79\x28\x39\x6A\x28\xF6\x7A\xDD\xEE\x8C\xA2\x31\xBD\x31"
            "\x31\x43\xCF\x38\x32\x3F\x90\xEB\x5F\xA4\xF2\xD6\x00\xE0\xA5\x78"
            "\xB5\x34\xBC\x2F\xDE\x75\x58\xAC\x1A\x92\x56\xEE\x7F\x5C\x79\x48"
            "\xE9\xD4\x59\x98\x90\x40\xAD\x61\x40\x2D\xC1\x79\x17\xEE\x18\x69"
            "\xD5\x89\x8A\x99\xF6\x45\xDF\x87\x31\xC7\x4B\xCF\x1A\xE8\x85\x81"
            "\x76\x60\x2E\x10\x5E\xE5\x0B\x29\x85\xA1\x53\xAC\x02\x9C\x59\xF9"
            "\x91\x5D\x8F\x26\x0F\x6D\x3C\xF5\x9B\xCB\x78\xCB\x81\x25\xF0\xB6"
            "\x86\x0A\xD6\x9D\x9E\xFF\x0A\xDA\xFC\x80\x06\x84\x92\xC9\x61\xB5"
            "\x51\xE3\x33\x61\xAB\x83\xEA\xC4\xEC\xC1\xDC\xBA\x91\x1E\x8B\x92"
            "\xBC\x87\x9F\x91\x56\x42\xCB\xD0\xBB\x48\x45\xBD\xA2\xAA\xC8\x8D"
            "\x98\x1E\x1A\xE0\x9C\xAA\xF6\x42\x3E\xE4\x38\xB7\xFD\xD4\x58\xF1"
            "\x6E\x21\x1F\xB6\x3A\x08\x03\x29\x16\x2A\xCC\xF8\x5A\x56\xD4\x66"
            "\x21\x52\x41\x54\x71\x1B\x1F\x93\xCC\xE8\x12\x71\x4A\xFD\x95\xE3"
            "\xEC\x92\x82\x76\x06\xCD\x04\x00\x3B\x43\xE9\x6E\x4A\x86\xD1\x5D"
            "\x4D\x12\x00\x08\xDF\x28\xB1\xDF\xB8\xDA\xBC\xC7\x9D\xD3\xF2\xE4"
            "\x03\x54\xB7\x0E\xC8\x34\xE5\x7C\x18\xD8\xB9\x62\x6B\x28\x0C\x9D"
            "\x2D\xB9\xBF\x7C\x1B\x9B\x0D\x60\x6A\xCA\x9F\x0F\xDB\xCA\x5C\x5B"
            "\x1E\xA1\xB0\xDD\xB9\x50\x03\x3F\x3A\x26\x12\xFC\x5C\xB6\xD0\x9C"
            "\xB8\xD0\x40\xA1\x37\x0C\xE0\x36\x70\xD5\x0B\x70\x54\xE1\x25\xF7"
            "\x78\xD1\x8D\x8E\xB8\xB2\xAC\xD1\x88\xFC\x8C\x8A\x58\xBE\x44\x71"
            "\x18\xF1\xBD\xB7\x5B\xB4\x28\x40\x0C\xBB\xD1\xA6\x7A\x20\xD8\x7A"
            "\x05\x2E\xBF\xC9\x12\xED\x2F\xD6\x6E\x5D\x30\xE9\xEC\x83\x7A\x89"
            "\xDA\x65\x82\x19\x4B\x7C\x45\x50\x51\xE3\x34\x36\x68\xA2\x75\xE5"
            "\x05\xDA\x71\x09\x51\x9E\x85\x9C\x22\xFE\xE2\xF5\x18\x27\xE5\x22"
            "\xCA\x71\x16\x77\xA6\xB9\x41\x03\x1A\xA7\x97\x51\xE3\xDD\x1E\x6F"
            "\xCD\x84\xCE\xEE\x7B\x57\x7C\x61\x03\x51\x8F\x08\xB0\x67\x85\x59"
            "\xD0\x4D\xCF\x63\xB1\xC1\x9F\xAB\x47\x37\x6C\xD4\xAE\xF3\xA7\x9D"
            "\x84\xB6\x73\x06\x76\xB0\x44\xB5\x81\xF8\x36\x87\xC0\x8D\xF2\x93"
            "\xCE\x36\xAE\x0B\x13\x7A\xCF\xEB\xDB\x5B\x39\x88\x00\xA8\x1E\xE4"
            "\x9A\x06\xD6\x56\x00\xB4\xA8\xBA\x57\x97\x69\x32\x96\x4B\x8F\x98"
            "\x66\x61\x3B\xE8\x5D\x50\xC5\x8B\x14\xE9\xC4\x02\x3A\xFC\xE5\x77"
            "\x09\xED\x3D\x17\x10\xB5\xF0\xD3\x2F\xF1\x5C\x9B\xD6\x37\x3C\x12"
            "\xFB\x01\xFE\x3B\x9F\xFF\xFF\xBC\x32\xF5\x8C\x67\x3C\xE3\x19\xCF"
            "\x78\xC6\x33\x9E\xF1\x8C\x67\x3C\xE3\x19\xCF\x78\xC6\x33\x9E\xF1"
            "\xB3\xE3\x97\x00\x03\x00\xDA\x70\x0D\x25\xB0\xD3\xC3\xB0\x00\x00"
            "\x00\x00\x49\x45\x4E\x44\xAE\x42\x60\x82");
        size_t size = 0x9DE;
        Texture* texture = new Texture(id, data, size, Texture::IF_R, false, Texture::FM_Nearest, Texture::FM_Nearest);
        textures_.insert(std::make_pair(texture->getId().resource, std::unique_ptr<Texture>(texture)));
    }
    catch (const std::exception& err)
    {
        logWarning("Texture", id, err.what());
    }

    id.resource = Id("TextureFont.default");
    try
    {
        const Texture& tex = getTexture(Id("Texture.TextureFont.default"));

        std::string src =
            "<?xml version=\"1.0\"?>\n"
            "<font>\n"
            "  <info face=\"standard 07_51\" />\n"
            "  <common lineHeight=\"12\" base=\"10\" scaleW=\"128\" scaleH=\"128\" />\n"
            "  <chars count=\"253\">\n"
            "    <char id=\"-1\" x=\"114\" y=\"11\" width=\"3\" height=\"9\" xoffset=\"0\" yoffset=\"1\" xadvance=\"4\" />\n"
            "    <char id=\"0\" x=\"81\" y=\"88\" width=\"1\" height=\"1\" xoffset=\"0\" yoffset=\"0\" xadvance=\"0\" />\n"
            "    <char id=\"32\" x=\"87\" y=\"88\" width=\"1\" height=\"1\" xoffset=\"0\" yoffset=\"0\" xadvance=\"4\" />\n"
            "    <char id=\"33\" x=\"126\" y=\"62\" width=\"1\" height=\"7\" xoffset=\"1\" yoffset=\"3\" xadvance=\"4\" />\n"
            "    <char id=\"34\" x=\"18\" y=\"91\" width=\"3\" height=\"2\" xoffset=\"0\" yoffset=\"3\" xadvance=\"5\" />\n"
            "    <char id=\"35\" x=\"97\" y=\"54\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"36\" x=\"36\" y=\"11\" width=\"7\" height=\"9\" xoffset=\"0\" yoffset=\"2\" xadvance=\"8\" />\n"
            "    <char id=\"37\" x=\"26\" y=\"40\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"9\" />\n"
            "    <char id=\"38\" x=\"34\" y=\"40\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"39\" x=\"25\" y=\"91\" width=\"1\" height=\"2\" xoffset=\"0\" yoffset=\"3\" xadvance=\"2\" />\n"
            "    <char id=\"40\" x=\"102\" y=\"11\" width=\"3\" height=\"9\" xoffset=\"0\" yoffset=\"2\" xadvance=\"4\" />\n"
            "    <char id=\"41\" x=\"118\" y=\"11\" width=\"3\" height=\"9\" xoffset=\"0\" yoffset=\"2\" xadvance=\"4\" />\n"
            "    <char id=\"42\" x=\"42\" y=\"80\" width=\"5\" height=\"5\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"43\" x=\"36\" y=\"80\" width=\"5\" height=\"5\" xoffset=\"0\" yoffset=\"4\" xadvance=\"6\" />\n"
            "    <char id=\"44\" x=\"110\" y=\"82\" width=\"2\" height=\"3\" xoffset=\"0\" yoffset=\"9\" xadvance=\"3\" />\n"
            "    <char id=\"45\" x=\"50\" y=\"90\" width=\"4\" height=\"1\" xoffset=\"0\" yoffset=\"6\" xadvance=\"5\" />\n"
            "    <char id=\"46\" x=\"89\" y=\"88\" width=\"1\" height=\"1\" xoffset=\"0\" yoffset=\"9\" xadvance=\"2\" />\n"
            "    <char id=\"47\" x=\"53\" y=\"30\" width=\"4\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"5\" />\n"
            "    <char id=\"48\" x=\"50\" y=\"39\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"49\" x=\"52\" y=\"71\" width=\"2\" height=\"7\" xoffset=\"2\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"50\" x=\"58\" y=\"39\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"51\" x=\"66\" y=\"39\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"52\" x=\"74\" y=\"39\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"53\" x=\"82\" y=\"38\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"54\" x=\"90\" y=\"38\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"55\" x=\"98\" y=\"38\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"56\" x=\"0\" y=\"49\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"57\" x=\"106\" y=\"38\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"58\" x=\"13\" y=\"87\" width=\"1\" height=\"4\" xoffset=\"1\" yoffset=\"5\" xadvance=\"4\" />\n"
            "    <char id=\"59\" x=\"49\" y=\"71\" width=\"2\" height=\"7\" xoffset=\"0\" yoffset=\"5\" xadvance=\"4\" />\n"
            "    <char id=\"60\" x=\"11\" y=\"73\" width=\"4\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"5\" />\n"
            "    <char id=\"61\" x=\"42\" y=\"86\" width=\"5\" height=\"3\" xoffset=\"0\" yoffset=\"5\" xadvance=\"7\" />\n"
            "    <char id=\"62\" x=\"16\" y=\"73\" width=\"4\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"5\" />\n"
            "    <char id=\"63\" x=\"63\" y=\"55\" width=\"6\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"7\" />\n"
            "    <char id=\"64\" x=\"0\" y=\"41\" width=\"8\" height=\"7\" xoffset=\"0\" yoffset=\"4\" xadvance=\"9\" />\n"
            "    <char id=\"65\" x=\"8\" y=\"49\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"66\" x=\"16\" y=\"49\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"67\" x=\"24\" y=\"48\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"68\" x=\"32\" y=\"48\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"69\" x=\"40\" y=\"48\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"70\" x=\"48\" y=\"47\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"71\" x=\"56\" y=\"47\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"72\" x=\"64\" y=\"47\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"73\" x=\"59\" y=\"71\" width=\"1\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"2\" />\n"
            "    <char id=\"74\" x=\"72\" y=\"47\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"75\" x=\"80\" y=\"47\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"76\" x=\"70\" y=\"55\" width=\"6\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"7\" />\n"
            "    <char id=\"77\" x=\"118\" y=\"30\" width=\"8\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"9\" />\n"
            "    <char id=\"78\" x=\"88\" y=\"46\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"79\" x=\"96\" y=\"46\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"80\" x=\"104\" y=\"46\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"81\" x=\"40\" y=\"21\" width=\"7\" height=\"8\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"82\" x=\"112\" y=\"46\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"83\" x=\"42\" y=\"39\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"84\" x=\"0\" y=\"57\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"85\" x=\"8\" y=\"57\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"86\" x=\"16\" y=\"57\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"87\" x=\"108\" y=\"30\" width=\"9\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"10\" />\n"
            "    <char id=\"88\" x=\"24\" y=\"56\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"89\" x=\"32\" y=\"56\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"90\" x=\"40\" y=\"56\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"91\" x=\"3\" y=\"22\" width=\"2\" height=\"9\" xoffset=\"0\" yoffset=\"2\" xadvance=\"4\" />\n"
            "    <char id=\"92\" x=\"63\" y=\"30\" width=\"4\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"5\" />\n"
            "    <char id=\"93\" x=\"6\" y=\"22\" width=\"2\" height=\"9\" xoffset=\"0\" yoffset=\"2\" xadvance=\"4\" />\n"
            "    <char id=\"94\" x=\"24\" y=\"87\" width=\"5\" height=\"3\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"95\" x=\"27\" y=\"91\" width=\"6\" height=\"1\" xoffset=\"0\" yoffset=\"9\" xadvance=\"7\" />\n"
            "    <char id=\"96\" x=\"22\" y=\"91\" width=\"2\" height=\"2\" xoffset=\"0\" yoffset=\"3\" xadvance=\"3\" />\n"
            "    <char id=\"97\" x=\"60\" y=\"79\" width=\"5\" height=\"5\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"98\" x=\"6\" y=\"65\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"99\" x=\"54\" y=\"79\" width=\"5\" height=\"5\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"100\" x=\"12\" y=\"65\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"101\" x=\"24\" y=\"81\" width=\"5\" height=\"5\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"102\" x=\"33\" y=\"72\" width=\"3\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"4\" />\n"
            "    <char id=\"103\" x=\"18\" y=\"65\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"104\" x=\"24\" y=\"64\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"105\" x=\"63\" y=\"71\" width=\"1\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"2\" />\n"
            "    <char id=\"106\" x=\"0\" y=\"22\" width=\"2\" height=\"9\" xoffset=\"0\" yoffset=\"3\" xadvance=\"3\" />\n"
            "    <char id=\"107\" x=\"30\" y=\"64\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"108\" x=\"61\" y=\"71\" width=\"1\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"2\" />\n"
            "    <char id=\"109\" x=\"99\" y=\"70\" width=\"9\" height=\"5\" xoffset=\"0\" yoffset=\"5\" xadvance=\"10\" />\n"
            "    <char id=\"110\" x=\"102\" y=\"76\" width=\"5\" height=\"5\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"111\" x=\"6\" y=\"81\" width=\"5\" height=\"5\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"112\" x=\"36\" y=\"64\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"113\" x=\"42\" y=\"64\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"114\" x=\"0\" y=\"87\" width=\"4\" height=\"5\" xoffset=\"0\" yoffset=\"5\" xadvance=\"5\" />\n"
            "    <char id=\"115\" x=\"18\" y=\"81\" width=\"5\" height=\"5\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"116\" x=\"25\" y=\"72\" width=\"3\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"4\" />\n"
            "    <char id=\"117\" x=\"72\" y=\"78\" width=\"5\" height=\"5\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"118\" x=\"78\" y=\"78\" width=\"5\" height=\"5\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"119\" x=\"119\" y=\"70\" width=\"7\" height=\"5\" xoffset=\"0\" yoffset=\"5\" xadvance=\"8\" />\n"
            "    <char id=\"120\" x=\"0\" y=\"81\" width=\"5\" height=\"5\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"121\" x=\"102\" y=\"62\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"122\" x=\"96\" y=\"77\" width=\"5\" height=\"5\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"123\" x=\"98\" y=\"11\" width=\"3\" height=\"9\" xoffset=\"0\" yoffset=\"2\" xadvance=\"4\" />\n"
            "    <char id=\"124\" x=\"126\" y=\"11\" width=\"1\" height=\"8\" xoffset=\"1\" yoffset=\"3\" xadvance=\"4\" />\n"
            "    <char id=\"125\" x=\"110\" y=\"11\" width=\"3\" height=\"9\" xoffset=\"0\" yoffset=\"2\" xadvance=\"4\" />\n"
            "    <char id=\"126\" x=\"115\" y=\"82\" width=\"4\" height=\"2\" xoffset=\"0\" yoffset=\"3\" xadvance=\"5\" />\n"
            "    <char id=\"160\" x=\"83\" y=\"88\" width=\"1\" height=\"1\" xoffset=\"0\" yoffset=\"0\" xadvance=\"3\" />\n"
            "    <char id=\"161\" x=\"57\" y=\"71\" width=\"1\" height=\"7\" xoffset=\"1\" yoffset=\"3\" xadvance=\"4\" />\n"
            "    <char id=\"162\" x=\"96\" y=\"62\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"163\" x=\"84\" y=\"55\" width=\"6\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"7\" />\n"
            "    <char id=\"164\" x=\"48\" y=\"55\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"165\" x=\"78\" y=\"63\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"166\" x=\"55\" y=\"71\" width=\"1\" height=\"7\" xoffset=\"1\" yoffset=\"3\" xadvance=\"4\" />\n"
            "    <char id=\"167\" x=\"6\" y=\"73\" width=\"4\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"5\" />\n"
            "    <char id=\"168\" x=\"73\" y=\"88\" width=\"3\" height=\"1\" xoffset=\"1\" yoffset=\"3\" xadvance=\"5\" />\n"
            "    <char id=\"169\" x=\"114\" y=\"38\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"170\" x=\"54\" y=\"63\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"171\" x=\"30\" y=\"86\" width=\"5\" height=\"3\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"172\" x=\"48\" y=\"86\" width=\"5\" height=\"3\" xoffset=\"0\" yoffset=\"6\" xadvance=\"6\" />\n"
            "    <char id=\"173\" x=\"45\" y=\"90\" width=\"4\" height=\"1\" xoffset=\"0\" yoffset=\"6\" xadvance=\"5\" />\n"
            "    <char id=\"174\" x=\"120\" y=\"46\" width=\"7\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"175\" x=\"65\" y=\"89\" width=\"3\" height=\"1\" xoffset=\"0\" yoffset=\"4\" xadvance=\"4\" />\n"
            "    <char id=\"176\" x=\"83\" y=\"84\" width=\"3\" height=\"3\" xoffset=\"0\" yoffset=\"3\" xadvance=\"4\" />\n"
            "    <char id=\"177\" x=\"71\" y=\"71\" width=\"5\" height=\"6\" xoffset=\"0\" yoffset=\"4\" xadvance=\"6\" />\n"
            "    <char id=\"178\" x=\"120\" y=\"76\" width=\"4\" height=\"5\" xoffset=\"0\" yoffset=\"3\" xadvance=\"5\" />\n"
            "    <char id=\"179\" x=\"5\" y=\"87\" width=\"3\" height=\"5\" xoffset=\"0\" yoffset=\"3\" xadvance=\"4\" />\n"
            "    <char id=\"180\" x=\"125\" y=\"82\" width=\"2\" height=\"2\" xoffset=\"1\" yoffset=\"2\" xadvance=\"4\" />\n"
            "    <char id=\"181\" x=\"121\" y=\"54\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"182\" x=\"48\" y=\"63\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"183\" x=\"91\" y=\"87\" width=\"1\" height=\"1\" xoffset=\"1\" yoffset=\"6\" xadvance=\"4\" />\n"
            "    <char id=\"184\" x=\"87\" y=\"84\" width=\"3\" height=\"3\" xoffset=\"0\" yoffset=\"9\" xadvance=\"4\" />\n"
            "    <char id=\"185\" x=\"125\" y=\"76\" width=\"2\" height=\"5\" xoffset=\"0\" yoffset=\"3\" xadvance=\"3\" />\n"
            "    <char id=\"186\" x=\"84\" y=\"63\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"187\" x=\"54\" y=\"85\" width=\"5\" height=\"3\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"188\" x=\"30\" y=\"22\" width=\"9\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"10\" />\n"
            "    <char id=\"189\" x=\"20\" y=\"22\" width=\"9\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"10\" />\n"
            "    <char id=\"190\" x=\"9\" y=\"22\" width=\"10\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"11\" />\n"
            "    <char id=\"191\" x=\"60\" y=\"63\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"192\" x=\"8\" y=\"11\" width=\"7\" height=\"10\" xoffset=\"0\" yoffset=\"0\" xadvance=\"8\" />\n"
            "    <char id=\"193\" x=\"0\" y=\"11\" width=\"7\" height=\"10\" xoffset=\"0\" yoffset=\"0\" xadvance=\"8\" />\n"
            "    <char id=\"194\" x=\"120\" y=\"0\" width=\"7\" height=\"10\" xoffset=\"0\" yoffset=\"0\" xadvance=\"8\" />\n"
            "    <char id=\"195\" x=\"112\" y=\"0\" width=\"7\" height=\"10\" xoffset=\"0\" yoffset=\"0\" xadvance=\"8\" />\n"
            "    <char id=\"196\" x=\"52\" y=\"11\" width=\"7\" height=\"9\" xoffset=\"0\" yoffset=\"1\" xadvance=\"8\" />\n"
            "    <char id=\"197\" x=\"104\" y=\"0\" width=\"7\" height=\"10\" xoffset=\"0\" yoffset=\"0\" xadvance=\"8\" />\n"
            "    <char id=\"198\" x=\"88\" y=\"30\" width=\"9\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"10\" />\n"
            "    <char id=\"199\" x=\"84\" y=\"11\" width=\"7\" height=\"9\" xoffset=\"0\" yoffset=\"3\" xadvance=\"8\" />\n"
            "    <char id=\"200\" x=\"96\" y=\"0\" width=\"7\" height=\"10\" xoffset=\"0\" yoffset=\"0\" xadvance=\"8\" />\n"
            "    <char id=\"201\" x=\"88\" y=\"0\" width=\"7\" height=\"10\" xoffset=\"0\" yoffset=\"0\" xadvance=\"8\" />\n"
            "    <char id=\"202\" x=\"80\" y=\"0\" width=\"7\" height=\"10\" xoffset=\"0\" yoffset=\"0\" xadvance=\"8\" />\n"
            "    <char id=\"203\" x=\"76\" y=\"11\" width=\"7\" height=\"9\" xoffset=\"0\" yoffset=\"1\" xadvance=\"8\" />\n"
            "    <char id=\"204\" x=\"33\" y=\"11\" width=\"2\" height=\"10\" xoffset=\"-1\" yoffset=\"0\" xadvance=\"2\" />\n"
            "    <char id=\"205\" x=\"30\" y=\"11\" width=\"2\" height=\"10\" xoffset=\"0\" yoffset=\"0\" xadvance=\"2\" />\n"
            "    <char id=\"206\" x=\"26\" y=\"11\" width=\"3\" height=\"10\" xoffset=\"-1\" yoffset=\"0\" xadvance=\"2\" />\n"
            "    <char id=\"207\" x=\"106\" y=\"11\" width=\"3\" height=\"9\" xoffset=\"-1\" yoffset=\"1\" xadvance=\"2\" />\n"
            "    <char id=\"208\" x=\"9\" y=\"41\" width=\"8\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"9\" />\n"
            "    <char id=\"209\" x=\"0\" y=\"0\" width=\"7\" height=\"10\" xoffset=\"0\" yoffset=\"0\" xadvance=\"8\" />\n"
            "    <char id=\"210\" x=\"16\" y=\"0\" width=\"7\" height=\"10\" xoffset=\"0\" yoffset=\"0\" xadvance=\"8\" />\n"
            "    <char id=\"211\" x=\"32\" y=\"0\" width=\"7\" height=\"10\" xoffset=\"0\" yoffset=\"0\" xadvance=\"8\" />\n"
            "    <char id=\"212\" x=\"72\" y=\"0\" width=\"7\" height=\"10\" xoffset=\"0\" yoffset=\"0\" xadvance=\"8\" />\n"
            "    <char id=\"213\" x=\"61\" y=\"21\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"6\" />\n"
            "    <char id=\"214\" x=\"44\" y=\"11\" width=\"7\" height=\"9\" xoffset=\"0\" yoffset=\"1\" xadvance=\"8\" />\n"
            "    <char id=\"215\" x=\"66\" y=\"78\" width=\"5\" height=\"5\" xoffset=\"0\" yoffset=\"4\" xadvance=\"6\" />\n"
            "    <char id=\"216\" x=\"72\" y=\"63\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"217\" x=\"64\" y=\"0\" width=\"7\" height=\"10\" xoffset=\"0\" yoffset=\"0\" xadvance=\"8\" />\n"
            "    <char id=\"218\" x=\"48\" y=\"0\" width=\"7\" height=\"10\" xoffset=\"0\" yoffset=\"0\" xadvance=\"8\" />\n"
            "    <char id=\"219\" x=\"24\" y=\"0\" width=\"7\" height=\"10\" xoffset=\"0\" yoffset=\"0\" xadvance=\"8\" />\n"
            "    <char id=\"220\" x=\"68\" y=\"11\" width=\"7\" height=\"9\" xoffset=\"0\" yoffset=\"1\" xadvance=\"8\" />\n"
            "    <char id=\"221\" x=\"56\" y=\"0\" width=\"7\" height=\"10\" xoffset=\"0\" yoffset=\"0\" xadvance=\"8\" />\n"
            "    <char id=\"222\" x=\"67\" y=\"21\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"223\" x=\"48\" y=\"21\" width=\"6\" height=\"8\" xoffset=\"0\" yoffset=\"3\" xadvance=\"7\" />\n"
            "    <char id=\"224\" x=\"73\" y=\"21\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"6\" />\n"
            "    <char id=\"225\" x=\"79\" y=\"21\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"6\" />\n"
            "    <char id=\"226\" x=\"85\" y=\"21\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"6\" />\n"
            "    <char id=\"227\" x=\"91\" y=\"21\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"6\" />\n"
            "    <char id=\"228\" x=\"66\" y=\"63\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"229\" x=\"97\" y=\"21\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"6\" />\n"
            "    <char id=\"230\" x=\"89\" y=\"71\" width=\"9\" height=\"5\" xoffset=\"0\" yoffset=\"5\" xadvance=\"10\" />\n"
            "    <char id=\"231\" x=\"0\" y=\"65\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"232\" x=\"103\" y=\"21\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"6\" />\n"
            "    <char id=\"233\" x=\"109\" y=\"21\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"6\" />\n"
            "    <char id=\"234\" x=\"115\" y=\"21\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"6\" />\n"
            "    <char id=\"235\" x=\"115\" y=\"54\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"236\" x=\"72\" y=\"30\" width=\"2\" height=\"8\" xoffset=\"-1\" yoffset=\"2\" xadvance=\"2\" />\n"
            "    <char id=\"237\" x=\"75\" y=\"30\" width=\"2\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"2\" />\n"
            "    <char id=\"238\" x=\"68\" y=\"30\" width=\"3\" height=\"8\" xoffset=\"-1\" yoffset=\"2\" xadvance=\"2\" />\n"
            "    <char id=\"239\" x=\"21\" y=\"73\" width=\"3\" height=\"7\" xoffset=\"-1\" yoffset=\"3\" xadvance=\"2\" />\n"
            "    <char id=\"240\" x=\"103\" y=\"54\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"241\" x=\"121\" y=\"21\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"6\" />\n"
            "    <char id=\"242\" x=\"0\" y=\"32\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"6\" />\n"
            "    <char id=\"243\" x=\"6\" y=\"32\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"6\" />\n"
            "    <char id=\"244\" x=\"12\" y=\"31\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"6\" />\n"
            "    <char id=\"245\" x=\"18\" y=\"31\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"6\" />\n"
            "    <char id=\"246\" x=\"109\" y=\"54\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"247\" x=\"48\" y=\"80\" width=\"5\" height=\"5\" xoffset=\"0\" yoffset=\"4\" xadvance=\"6\" />\n"
            "    <char id=\"248\" x=\"30\" y=\"80\" width=\"5\" height=\"5\" xoffset=\"0\" yoffset=\"5\" xadvance=\"6\" />\n"
            "    <char id=\"249\" x=\"24\" y=\"31\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"6\" />\n"
            "    <char id=\"250\" x=\"30\" y=\"31\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"6\" />\n"
            "    <char id=\"251\" x=\"36\" y=\"31\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"2\" xadvance=\"6\" />\n"
            "    <char id=\"252\" x=\"90\" y=\"63\" width=\"5\" height=\"7\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "    <char id=\"253\" x=\"16\" y=\"11\" width=\"5\" height=\"10\" xoffset=\"0\" yoffset=\"2\" xadvance=\"6\" />\n"
            "    <char id=\"254\" x=\"55\" y=\"21\" width=\"5\" height=\"8\" xoffset=\"0\" yoffset=\"4\" xadvance=\"6\" />\n"
            "    <char id=\"255\" x=\"92\" y=\"11\" width=\"5\" height=\"9\" xoffset=\"0\" yoffset=\"3\" xadvance=\"6\" />\n"
            "  </chars>\n"
            "</font>\n";

        texture_fonts_.insert(std::make_pair(id.resource, std::move(sw::parseBmFontXml(id, tex.getHandle(), src.data(), src.size()))));
    }
    catch (const std::exception& err)
    {
        logWarning("TextureFont", id, err.what());
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
