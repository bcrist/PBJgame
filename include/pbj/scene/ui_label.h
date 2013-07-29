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
/// \file   pbj/scene/ui_label.h
/// \author Benjamin Crist
///
/// \brief  pbj::scene::UILabel class header.

#ifndef PBJ_SCENE_UI_LABEL_H_
#define PBJ_SCENE_UI_LABEL_H_

#include "pbj/scene/ui_element.h"
#include "pbj/gfx/texture_font.h"
#include "pbj/gfx/texture_font_text.h"
#include "be/const_handle.h"

namespace pbj {
namespace scene {

class UIButton;

///////////////////////////////////////////////////////////////////////////////
/// \brief  Text Label UI element.
class UILabel : public UIElement
{
    friend class UIButton;

public:
    enum Align
    {
        AlignLeft,
        AlignCenter,
        AlignRight,
    };

    UILabel();
    virtual ~UILabel();

    virtual UIElement* getElementAt(const ivec2& screen_position);

    void setText(const std::string& text);
    const std::string& getText() const;

    void setTextScale(const vec2& scale);
    const vec2& getTextScale() const;

    void setTextColor(const color4& color);
    const color4& getTextColor() const;

    void setFont(const be::ConstHandle<gfx::TextureFont>& font);
    const be::ConstHandle<gfx::TextureFont>& getFont() const;

    void setAlign(Align align);
    Align getAlign() const;

    virtual void draw();

private:
    virtual void onBoundsChange_();

    void calculateTextTransform_();

    vec2 text_scale_;
    gfx::TextureFontText tf_text_;
    Align align_;

    mat4 text_transform_;
    bool text_transform_valid_;

    UILabel(const UILabel&);
    void operator=(const UILabel&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
