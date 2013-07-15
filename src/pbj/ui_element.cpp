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
/// \file   pbj/ui_element.cpp
/// \author Josh Douglas
///
/// \brief  pbj::UIElement class source.

#include "pbj\ui_element.h"

namespace pbj {

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

    void UIElement::setNextFocusableElement(UIElement* element)
    {
       next_focus_ = element;
    }

    UIElement* UIElement::getNextFocusableElement()
    {
        return next_focus_;
    }

} // namespace pbj
