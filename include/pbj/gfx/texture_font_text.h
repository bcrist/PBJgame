///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/texture_font_text.h
/// \author Benjamin Crist
///
/// \brief  pbj::gfx::TextureFontText class header.

#ifndef PBJ_GFX_TEXTURE_FONT_TEXT_H_
#define PBJ_GFX_TEXTURE_FONT_TEXT_H_

#include "pbj/gfx/texture_font.h"

#include "pbj/gfx/batcher.h"

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
/// \brief  A mesh-like object which represents a specific text string rendered
///         using a particular TextureFont.
class TextureFontText
{
public:
    TextureFontText();
    ~TextureFontText();

    void setBufferMode(GLenum buffer_mode);
    GLenum getBufferMode() const;

    void setColor(const vec4& color);
    const vec4& getColor() const;

    void setFont(const be::ConstHandle<TextureFont>& font);
    const be::ConstHandle<TextureFont>& getFont() const;

    void setText(const std::string& text);
    const std::string& getText() const;
    F32 getTextWidth();

    void setOrderIndex(U32 order_index);
    U32 getOrderIndex() const;

    void prepare();
    bool isPrepared() const;

    void draw(const mat4* transform, const ScissorConfig* scissor);

private:
    vec4 color_;
   
    std::string text_;
    
    GLenum buffer_mode_;

    be::ConstHandle<Texture> texture_;
    be::ConstHandle<TextureFont> font_;

    SamplerConfig sampler_;
    UniformConfig uniforms_[2];
    BatcherTask btask_;
    GLuint ibo_id_; ///< OpenGL Vertex Index buffer object id
    GLuint vbo_id_; ///< OpenGL Vertex buffer object id

    bool buffers_valid_;
    F32 text_width_;

    TextureFontText(const TextureFontText&);
    void operator=(const TextureFontText&);
};

} // namespace pbj::gfx
} // namespace pbj

#endif
