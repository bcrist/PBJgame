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
/// \file   pbj/scene/ui_root.cpp
/// \author Ben Crist

#include "pbj/scene/ui_root.h"

#include "pbj/engine.h"
#include "pbj/window.h"
#include "pbj/input_controller.h"

#include <cassert>

namespace pbj {
namespace scene {

UIRoot::UIRoot()
    : under_mouse_(nullptr),
      focused_element_(nullptr),
      button1_down_over_(nullptr),
      button2_down_over_(nullptr),
      button3_down_over_(nullptr)
{
    handle_.associate(this);
    be::Handle<UIRoot> handle = handle_;

    InputController::registerMouseMotionListener(
        [=](F64 x, F64 y)
        {
            UIRoot* root = handle.get();
            if (root)
                root->onMouseMove(ivec2(x, y));
        }
    );

    InputController::registerMouseButtonAnyListener(
        [=](I32 button, I32 action, I32 mods)
        {
            UIRoot* root = handle.get();
            if (root)
                root->onMouseButton(button, action == GLFW_PRESS);
        }
    );

    InputController::registerKeyAllListener(
        [=](I32 keycode, I32 scancode, I32 action, I32 modifiers)
        {
            UIRoot* root = handle.get();
            if (root)
                root->onKey(keycode, action, modifiers);
        }
    );

    InputController::registerCharInputListener(
        [=](U32 codepoint)
        {
            UIRoot* root = handle.get();
            if (root)
                root->onCharacter(codepoint);
        }
    );

    Window* window = getEngine().getWindow();

    assert(window);

    window->registerContextResizeListener(
        [=](I32 width, I32 height)
        {
            UIRoot* root = handle.get();
            if (root)
            {
                root->projection_matrix_ = glm::ortho(0.0f, F32(width), F32(height), 0.0f);
                root->panel.onBoundsChange_();
            }
        }
    );

    projection_matrix_ = glm::ortho(0.0f, F32(window->getContextSize().x), F32(window->getContextSize().y), 0.0f);

    panel.projection_ = &projection_matrix_;
    panel.view_ = &view_matrix_;
    panel.inv_view_ = &view_matrix_;
    panel.focused_element_ = &focused_element_;
    panel.onBoundsChange_();
}

UIRoot::~UIRoot()
{
}

void UIRoot::draw()
{
    panel.draw();
}

void UIRoot::onMouseMove(const ivec2& position)
{
    mouse_position_ = position;
    UIElement* under_mouse = panel.getElementAt(position);

    if (under_mouse != under_mouse_)
    {
        if (under_mouse_)
            under_mouse_->onMouseOut(position);

        if (under_mouse)
            under_mouse->onMouseIn(position);

        under_mouse_ = under_mouse;
    }

    if (under_mouse_)
        under_mouse_->onMouseMove(position);

    if (button1_down_over_ && button1_down_over_ != under_mouse_)
        button1_down_over_->onMouseMove(position);

    if (button2_down_over_ && button2_down_over_ != under_mouse_ && button2_down_over_ != button1_down_over_)
        button2_down_over_->onMouseMove(position);

    if (button3_down_over_ && button3_down_over_ != under_mouse_ && button3_down_over_ != button1_down_over_ && button3_down_over_ != button2_down_over_)
        button3_down_over_->onMouseMove(position);
}

void UIRoot::onMouseButton(I32 button, bool down)
{
    UIElement* under_mouse = panel.getElementAt(mouse_position_);

    if (under_mouse)
    {
        if (down)
            under_mouse->onMouseDown(button);
        else
            under_mouse->onMouseUp(button);
    }

    UIElement** down_over = nullptr;
    if (button == GLFW_MOUSE_BUTTON_1)
        down_over = &button1_down_over_;
    else if (button == GLFW_MOUSE_BUTTON_2)
        down_over = &button2_down_over_;
    else if (button == GLFW_MOUSE_BUTTON_3)
        down_over = &button3_down_over_;

    if (down_over)
    {
        if (down)
        {
            *down_over = under_mouse;
        }
        else
        {
            // mouse up event for one of first 3 buttons

            if (*down_over && *down_over != under_mouse)
                (*down_over)->onMouseUp(button);
            else
            {
                under_mouse->onMouseClick(button);

                // TODO: dispatch double click event if 2 mouse clicks happen (same button) within ~350 ms.
            }

            *down_over = nullptr;
        }
    }
}

void UIRoot::onKey(I32 keycode, I32 action, I32 modifiers)
{
    if (!focused_element_)
        return;

    switch (action)
    {
        case GLFW_PRESS:
            focused_element_->onKeyDown(keycode, modifiers);
            focused_element_->onKeyPressed(keycode, modifiers);
            break;

        case GLFW_REPEAT:
            focused_element_->onKeyDown(keycode, modifiers);
            break;

        case GLFW_RELEASE:
            focused_element_->onKeyUp(keycode, modifiers);
            break;

        default:
            assert(false);  // no other action should ever be seen.
            break;
    }
}

void UIRoot::onCharacter(I32 codepoint)
{
    if (!focused_element_)
        return;

    focused_element_->onCharacter(codepoint);
}

} // namespace pbj::scene
} // namespace pbj
