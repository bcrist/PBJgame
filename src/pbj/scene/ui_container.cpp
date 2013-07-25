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
/// \file   pbj/scene/ui_container.cpp
/// \author Ben Crist

#include "pbj/scene/ui_container.h"

namespace pbj {
namespace scene {

UIContainer::UIContainer()
{
}

UIContainer::~UIContainer()
{
    focused_element_ = nullptr;
}

void UIContainer::addElement(std::unique_ptr<UIElement>&& element)
{
    elements_.push_back(std::move(element));
}

UIElement* UIContainer::getElementAt(const ivec2& position)
{
    if (isVisible() &&
        position.x >= position_.x && position.x < position_.x + dimensions_.x &&
        position.y >= position_.y && position.y < position_.y + dimensions_.y)
    {
        ivec2 client_position = position - position_;
        client_position.x /= scale_.x;
        client_position.y /= scale_.y;

        for (std::unique_ptr<UIElement>& ptr : elements_)
        {
            UIElement* element = ptr->getElementAt(client_position);
            if (element)
                return element;
        }

        //return this;
    }
    return nullptr;
}

void UIContainer::draw(const mat4& view_projection)
{
    if (isVisible())
    {
        mat4 combined = glm::scale(glm::translate(view_projection, vec3(getPosition(), 0)), vec3(scale_, 1));

        for (std::unique_ptr<UIElement>& ptr : elements_)
            ptr->draw(combined);
    }
}


} // namespace pbj::scene
} // namespace pbj
