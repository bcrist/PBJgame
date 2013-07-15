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
/// \file   pbj/ui_element.h
/// \author Benjamin Crist
///
/// \brief  pbj::UIElement class header.

#ifndef PBJ_UI_ELEMENT_H_
#define PBJ_UI_ELEMENT_H_

#include "pbj/_pbj.h"
#include "pbj/_math.h"

namespace pbj {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Abstract base class for UI elements.
///
/// \details UI origin is in top left corner of screen.
class UIElement
{
public:
    UIElement();
    UIElement(const ivec2& position, const ivec2& dimensions);
    virtual ~UIElement();

    void setVisible(bool visible);
    bool isVisible() const;

    void setEnabled(bool enabled);
    bool isEnabled() const;

    void setFocused(bool focused);
    bool isFocused() const;

    void setPosition(const ivec2& position);        ///< Set top left position of element.
    const ivec2& getPosition() const;               ///< Get top left position of element.

    void setDimensions(const ivec2& dimensions);    ///< Set width/height of element.
    const ivec2& getDimensions() const;             ///< Get width/height of element.

    // Retrieve the element at the given coordinates (check against element bounds)
    virtual UIElement* getElementAt(const ivec2& position);

    void setNextFocusableElement(UIElement* element);   ///< Set the element which should receive keyboard focus when this element is focused and Tab is pressed.
    UIElement* getNextFocusableElement();               ///< Get the element which should receive keyboard focus when this element is focused and Tab is pressed.

    virtual void onMouseIn(const ivec2& position);      ///< Called when the mouse moves into the element
    virtual void onMouseMove(const ivec2& position);    ///< Called when the mouse moves while over the element
    virtual void onMouseOut(const ivec2& position);     ///< Called when the mouse leaves the element

    virtual void onMouseDown(F32 button);               ///< Called when a mouse button is pressed while the mouse is over the element
    virtual void onMouseUp(F32 button);                 ///< Called when a mouse button is released while the mouse is over the element
    virtual void onMouseClick(F32 button);              ///< Called when a mouse button is pressed and released, both while the mouse is over the element
    virtual void onMouseDblClick(F32 button);           ///< Called when a mouse button is pressed and released twice, all while the mouse is over the element

    virtual void onKeyDown(F32 keycode);                ///< Called when a key is pressed while this element is focused
    virtual void onKeyUp(F32 keycode);                  ///< Called when a key is released while this element is focused
    virtual void onKeyPressed(F32 keycode);             ///< Called when a key is pressed and released, both while this element is focused
    virtual void onCharacter(F32 codepoint);            ///< Called when a character is input.  Not necessarily a 1:1 relationship with calls to onKeyPressed and provides a unicode codepoint rather than GLFW keycode.

    virtual void draw() = 0;

private:
    ivec2 position_;
    ivec2 dimensions_;

    UIElement* next_focus_;

    bool visible_;
    bool enabled_;
    bool focused_;

    UIElement(const UIElement&);
    void operator=(const UIElement&);
};

} // namespace pbj

#endif
