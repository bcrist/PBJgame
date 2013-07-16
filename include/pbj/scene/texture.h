// Copyright (c) 2013 PBJ Productions
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
/// \file   pbj/scene/texture.h
/// \author Benjamin Crist
///
/// \brief  pbj::scene::Texture class header.

#ifndef PBJ_SCENE_TEXTURE_H_
#define PBJ_SCENE_TEXTURE_H_

#include <map>
#include <vector>

#include "pbj/_pbj.h"
#include "pbj/_math.h"
#include "be/_gl.h"
#include "be/source_handle.h"
#include "be/asset_id.h"

namespace pbj {
namespace scene {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Represents an OpenGL texture object
class Texture
{
public:
    Texture();
    ~Texture();

    be::Handle<Texture> getHandle();
    const be::ConstHandle<Texture> getHandle() const;

    const AssetId& getId() const;

    GLuint getGlId() const;

#ifdef PBJ_EDITOR
    Texture();   // construct without uploading to GL

    void setName(const std::string& name);
    const std::string& getName() const;

    bool isValid() const;
    

    void upload();
#endif

private:
    void upload_(const GLubyte data);
    void invalidate_();

    be::SourceHandle<Texture> handle_;
    AssetId asset_id_;



    ivec2 dimensions_;
    GLuint gl_id_;
    
#ifdef PBJ_EDITOR
    std::string& nullString_() const;

    std::string name_;
    std::vector<U8> data_;
    std::map<std::string, std::string> metadata_;
#endif

    Texture(const Texture&);
    void operator=(const Texture&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
