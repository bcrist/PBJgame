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
/// \file   pbj/scene/ui_element.h
/// \author Benjamin Crist
///
/// \brief  pbj::scene::UIElement class header.

#ifndef PBJ_SCENE_UI_ELEMENT_H_
#define PBJ_SCENE_UI_ELEMENT_H_

#include "pbj/_pbj.h"
#include "pbj/_math.h"

#include "pbj/gfx/batcher.h"

namespace pbj {
namespace scene {

class UIPanel;
class UIRoot;

///////////////////////////////////////////////////////////////////////////////
/// \brief  Abstract base class for UI elements.
///
/// \details UI origin is in top left corner of screen.
class UIElement
{
    friend class UIRoot;
    friend class UIPanel;

public:
    UIElement();
    virtual ~UIElement();

    void setVisible(bool visible);
    bool isVisible() const;

    void setFocused();
    bool isFocused() const;
    virtual bool isFocusable() const;

    void setPosition(const vec2& position);
    const vec2& getPosition() const;

    void setDimensions(const vec2& dimensions);
    const vec2& getDimensions() const;

    virtual UIElement* getElementAt(const ivec2& screen_position);

    void setNextFocusElement(UIElement* element);
    UIElement* getNextFocusableElement();
    UIElement* getPreviousFocusableElement();

    virtual void onMouseIn(const ivec2& screen_position);
    virtual void onMouseMove(const ivec2& screen_position);
    virtual void onMouseOut(const ivec2& screen_position);

    virtual void onMouseDown(I32 button);
    virtual void onMouseUp(I32 button);
    virtual void onMouseClick(I32 button);
    virtual void onMouseDblClick(I32 button);

    virtual void onKeyDown(I32 keycode, I32 modifiers);
    virtual void onKeyUp(I32 keycode, I32 modifiers);
    virtual void onKeyPressed(I32 keycode, I32 modifiers);
    virtual void onCharacter(I32 codepoint);
   
    virtual void draw() = 0;

protected:
    virtual void onBoundsChange_();

    virtual void onFocusChange_();
    virtual void onVisibilityChange_();
    
    vec2 position_;
    vec2 dimensions_;

    const U32* order_index_offset_;
    const gfx::ScissorConfig* scissor_;
    const mat4* projection_;
    const mat4* view_;
    const mat4* inv_view_;
    

    UIElement** focused_element_;
    UIElement* next_focus_;

    bool visible_;

    UIElement(const UIElement&);
    void operator=(const UIElement&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
