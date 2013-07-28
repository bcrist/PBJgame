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
/// \file   pbj/scene/ui_panel.cpp
/// \author Ben Crist

#include "pbj/scene/ui_panel.h"

namespace pbj {
namespace scene {

UIPanel::UIPanel()
    : scale_(1.0f, 1.0f)
{
}

UIPanel::~UIPanel()
{
}

void UIPanel::setScale(const vec2& scale)
{
    if (scale != scale_)
    {
        scale_ = scale;
        onBoundsChange_();
    }
}

const vec2& UIPanel::getScale() const
{
    return scale_;
}

void UIPanel::addElement(std::unique_ptr<UIElement>&& element)
{
    element->projection_ = projection_;
    element->view_ = &view_matrix_;
    element->inv_view_ = &inv_view_matrix_;
    element->focused_element_ = focused_element_;
    element->onBoundsChange_();

    elements_.push_back(std::move(element));
}

UIElement* UIPanel::getElementAt(const ivec2& screen_position)
{
    if (!inv_view_ || !isVisible())
        return nullptr;

    for (std::unique_ptr<UIElement>& ptr : elements_)
    {
        UIElement* element = ptr->getElementAt(screen_position);
        if (element)
            return element;
    }

    /*vec2 pos = vec2(*inv_view_ * vec4(screen_position, 0, 1));

    if (pos.x >= position_.x && pos.x < position_.x + dimensions_.x &&
        pos.y >= position_.y && pos.y < position_.y + dimensions_.y)
    {
        return this;
    }*/
    return nullptr;
}

void UIPanel::draw()
{
    if (!isVisible())
        return;
    
    for (std::unique_ptr<UIElement>& ptr : elements_)
        ptr->draw();
}

void UIPanel::onBoundsChange_()
{
    view_matrix_ = glm::scale(glm::translate(*view_, vec3(getPosition(), 0)), vec3(scale_, 1)); 
    inv_view_matrix_ = glm::inverse(view_matrix_);

    for (std::unique_ptr<UIElement>& ptr : elements_)
    {
        ptr->projection_ = projection_;
        ptr->view_ = &view_matrix_;
        ptr->inv_view_ = &inv_view_matrix_;
        ptr->focused_element_ = focused_element_;
        ptr->onBoundsChange_();
    }
}

} // namespace pbj::scene
} // namespace pbj
