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
/// \file   pbj/scene/ui_label.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::scene::UILabel functions.

#include "pbj/scene/ui_label.h"

namespace pbj {
namespace scene {

UILabel::UILabel()
    : background_color_(0.0f, 0.0f, 0.0f, 0.0f),
      text_scale_(1.0f, 1.0f),
      align_(AlignLeft)
{
}

UILabel::~UILabel()
{
}

void UILabel::setText(const std::string& text)
{
    tf_text_.setText(text);
    calculateTextTransform();
}

const std::string& UILabel::getText() const
{
    return tf_text_.getText();
}

void UILabel::setTextScale(const vec2& scale)
{
    text_scale_ = scale;
    calculateTextTransform();
}

const vec2& UILabel::getTextScale() const
{
    return text_scale_;
}

void UILabel::setBackgroundColor(const color4& color)
{
    background_color_ = color;
}

const color4& UILabel::getBackgroundColor() const
{
    return background_color_;
}

void UILabel::setTextColor(const color4& color)
{
    tf_text_.setColor(color);
}

const color4& UILabel::getTextColor() const
{
    return tf_text_.getColor();
}

void UILabel::setFont(const be::ConstHandle<gfx::TextureFont>& font)
{
    tf_text_.setFont(font);
    calculateTextTransform();
}

const be::ConstHandle<gfx::TextureFont>& UILabel::getFont() const
{
    tf_text_.getFont();
}

void UILabel::setAlign(Align align)
{
    if (align != align_)
    {
        align_ = align;
        calculateTextTransform();
    }
}

UILabel::Align UILabel::getAlign() const
{
    return align_;
}

void UILabel::draw(const mat4& view_projection)
{
    tf_text_.draw(view_projection * text_transform_);
}

void UILabel::calculateTextTransform()
{
    const gfx::TextureFont* font = tf_text_.getFont().get();

    vec2 extra_space(getDimensions());
    extra_space -= vec2(tf_text_.getTextWidth() * text_scale_.x, font ? font->getCapHeight() * text_scale_.y : 0);

    extra_space *= 0.5f;

    I32 bottom_spacing = I32(extra_space.y);
    I32 left_spacing = I32(extra_space.x);

    if (align_ == AlignLeft)
        left_spacing = 0;

    if (align_ == AlignRight)
        left_spacing *= 2;

    vec2 translation(getPosition());
    translation.y += getDimensions().y - bottom_spacing;


}

} // namespace pbj::scene
} // namespace pbj
