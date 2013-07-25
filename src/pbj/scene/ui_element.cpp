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

#include "pbj/_gl.h"

namespace pbj {
namespace scene {

UIElement::UIElement()
    : visible_(true),
      focused_(false),
      next_focus_(nullptr)
{
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

void UIElement::setFocused(bool focused)
{
    if (focused != focused_ && next_focus_)
    {
        focused_ = focused;
        if (focused)
        {
            UIElement* element = next_focus_;
            while (element && element != this)
            {
                element->setFocused(false);
                element = element->getNextFocusableElement();
            }
            
            onFocusGained_();
        }
        else
        {
            onFocusLost_();
        }
    }
}

bool UIElement::isFocused() const
{
    return focused_;
}

void UIElement::setPosition(const ivec2& position)
{
    position_ = position;
    onBoundsChange_();
}

const ivec2& UIElement::getPosition() const
{
    return position_;
}

void UIElement::setDimensions(const ivec2& dimensions)
{
    dimensions_ = dimensions;
    onBoundsChange_();
}

const ivec2& UIElement::getDimensions() const
{
    return dimensions_;
}

UIElement* UIElement::getElementAt(const ivec2& position)
{
    if (isVisible() &&
        position.x >= position_.x && position.x < position_.x + dimensions_.x &&
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
	
void UIElement::onMouseDown(I32 button)
{
}
	
void UIElement::onMouseUp(I32 button)
{
}
	
void UIElement::onMouseClick(I32 button)
{
    if (next_focus_)
        setFocused(true);
}
	
void UIElement::onMouseDblClick(I32 button)
{
}

void UIElement::onKeyDown(I32 keycode, I32 modifiers)
{
}
	
void UIElement::onKeyUp(I32 keycode, I32 modifiers)
{
}
	
void UIElement::onKeyPressed(I32 keycode, I32 modifiers)
{
    if (next_focus_ && keycode == GLFW_KEY_TAB && (0 == (modifiers & (GLFW_MOD_ALT | GLFW_MOD_CONTROL | GLFW_MOD_SUPER))))
    {
        if (modifiers & GLFW_MOD_SHIFT)
        {
            UIElement* prev = next_focus_;
            while (prev->next_focus_ && prev->next_focus_ != this)
                prev = prev->next_focus_;

            prev->setFocused(true);
        }
        else
        {
            next_focus_->setFocused(true);
        }
    }
}
	
void UIElement::onCharacter(I32 codepoint)
{
}

void UIElement::onBoundsChange_()
{
}

void UIElement::onFocusGained_()
{
}

void UIElement::onFocusLost_()
{
}

} // namespace pbj::scene
} // namespace pbj
