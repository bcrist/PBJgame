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
/// \file   pbj/scene/ui_element.cpp
/// \author Josh Douglas
///
/// \brief  pbj::scene::UIElement class source.

#include "pbj/scene/ui_element.h"

namespace pbj {
namespace scene {

UIElement::UIElement()
{
    visible_ = false;
    enabled_ = false;
    focused_ = false;
    next_focus_ = NULL;

}

UIElement::UIElement(const ivec2& position, const ivec2& dimensions)
{
    position_ = position;
    dimensions_ = dimensions;
}

UIElement::~UIElement()
{
}

void UIElement::setVisible(bool visible)
{
    visible_ = visible;
}
    
bool UIElement::isVisible() const
{
    return visible_;
}

void UIElement::setEnabled(bool enabled)
{
    enabled_ = enabled;
}
    
bool UIElement::isEnabled() const
{
    return enabled_;
}

void UIElement::setFocused(bool focused)
{
    focused_ = focused;
}

bool UIElement::isFocused() const
{
    return focused_;
}

void UIElement::setPosition(const ivec2& position)
{
    position_ = position;
}

const ivec2& UIElement::getPosition() const
{
    return position_;
}

void UIElement::setDimensions(const ivec2& dimensions)
{
    dimensions_ = dimensions;
}

const ivec2& UIElement::getDimensions() const
{
    return dimensions_;
}

UIElement* UIElement::getElementAt(const ivec2& position)
{
    if (position.x >= position_.x && position.x < position_.x + dimensions_.x &&
        position.y >= position_.y && position.y < position_.y + dimensions_.y)
    {
        return this;
    }
    return nullptr;
}

void UIElement::setNextFocusableElement(UIElement* element)
{
    next_focus_ = element;
}

UIElement* UIElement::getNextFocusableElement()
{
    return next_focus_;
}

void UIElement::onMouseIn(const ivec2& position)
{
}
	
void UIElement::onMouseMove(const ivec2& position)
{
}
	
void UIElement::onMouseOut(const ivec2& position)
{
}
	
void UIElement::onMouseDown(F32 button)
{
}
	
void UIElement::onMouseUp(F32 button)
{
}
	
void UIElement::onMouseClick(F32 button)
{
}
	
void UIElement::onMouseDblClick(F32 button)
{
}

void UIElement::onKeyDown(F32 keycode)
{
}
	
void UIElement::onKeyUp(F32 keycode)
{
}
	
void UIElement::onKeyPressed(F32 keycode)
{
}
	
void UIElement::onCharacter(F32 codepoint)
{
}

} // namespace pbj::scene
} // namespace pbj
