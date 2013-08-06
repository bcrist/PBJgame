///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_label.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::scene::UILabel functions.

#include "pbj/scene/ui_label.h"

#include "pbj/_math.h"

#include <iostream>

namespace pbj {
namespace scene {

UILabel::UILabel()
    : text_scale_(1.0f, 1.0f),
      align_(AlignLeft),
      text_transform_valid_(false)
{
}

UILabel::~UILabel()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Determines if the point at the specified position resides within
///         this element (or one of its children).
///
/// \param  position The screen coordinates of the point in question.
/// \return A pointer to an element at these coordinates, or nullptr if there
///         is no such element.
UIElement* UILabel::getElementAt(const ivec2& screen_position)
{
    return nullptr;
}

void UILabel::setText(const std::string& text)
{
    tf_text_.setText(text);
    text_transform_valid_ = false;
}

const std::string& UILabel::getText() const
{
    return tf_text_.getText();
}

void UILabel::setTextScale(const vec2& scale)
{
    if (scale != text_scale_)
    {
        text_scale_ = scale;
        text_transform_valid_ = false;
    }
}

const vec2& UILabel::getTextScale() const
{
    return text_scale_;
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
    if (font != tf_text_.getFont())
    {
        tf_text_.setFont(font);
        text_transform_valid_ = false;
    }
}

const be::ConstHandle<gfx::TextureFont>& UILabel::getFont() const
{
    return tf_text_.getFont();
}

void UILabel::setAlign(Align align)
{
    if (align != align_)
    {
        align_ = align;
        text_transform_valid_ = false;
    }
}

UILabel::Align UILabel::getAlign() const
{
    return align_;
}

void UILabel::draw()
{
    if (isVisible() && view_ && projection_)
    {
        if (!text_transform_valid_)
            calculateTextTransform_();

        tf_text_.draw(&text_transform_, scissor_);
    }
}

void UILabel::onBoundsChange_()
{
    tf_text_.setOrderIndex(*order_index_offset_ + 8);
    text_transform_valid_ = false;
}

void UILabel::calculateTextTransform_()
{
    if (!(projection_ && view_))
        return;

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

    vec3 translation(getPosition(), 0);
    translation.y += getDimensions().y - bottom_spacing;
    translation.x += left_spacing;

    text_transform_ = glm::scale(glm::translate(*projection_ * *view_, translation), vec3(text_scale_.x, -text_scale_.y, 1));
    text_transform_valid_ = true;
}

} // namespace pbj::scene
} // namespace pbj
