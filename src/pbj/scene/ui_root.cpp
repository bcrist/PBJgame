///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_root.cpp
/// \author Ben Crist

#include "pbj/scene/ui_root.h"

#include "pbj/engine.h"
#include "pbj/window.h"
#include "pbj/input_controller.h"

#include <cassert>
#include <iostream>

namespace pbj {
namespace scene {

UIRoot::UIRoot()
    : under_mouse_(nullptr),
      focused_element_(&panel),
      button1_down_over_(nullptr),
      button2_down_over_(nullptr),
      button3_down_over_(nullptr),
      order_index_offset_(1000),
      input_handlers_active_(true)
{
    handle_.associate(this);

    mouse_motion_listener_id_ = InputController::registerMouseMotionListener(
        [&](F64 x, F64 y)
        {
            if (input_handlers_active_)
                onMouseMove(ivec2(x, y));
        }
    );

    mouse_button_any_listener_id_ = InputController::registerMouseButtonAnyListener(
        [&](I32 button, I32 action, I32 mods)
        {
            if (input_handlers_active_)
                onMouseButton(button, action == GLFW_PRESS);
        }
    );

    key_all_listener_id_ = InputController::registerKeyAllListener(
        [&](I32 keycode, I32 scancode, I32 action, I32 modifiers)
        {
            if (input_handlers_active_)
                onKey(keycode, action, modifiers);
        }
    );

    char_input_listener_id_ = InputController::registerCharInputListener(
        [&](U32 codepoint)
        {
            if (input_handlers_active_)
                onCharacter(codepoint);
        }
    );

    Window* window = getEngine().getWindow();
    assert(window);

    context_resize_listener_id_ = window->registerContextResizeListener(
        [&](I32 width, I32 height)
        {
            projection_matrix_ = glm::ortho(0.0f, F32(width), F32(height), 0.0f);
            panel.onBoundsChange_();
        }
    );

    projection_matrix_ = glm::ortho(0.0f, F32(window->getContextSize().x), F32(window->getContextSize().y), 0.0f);

    panel.scissor_ = nullptr;
    panel.order_index_offset_ = &order_index_offset_;
    panel.projection_ = &projection_matrix_;
    panel.view_ = &view_matrix_;
    panel.inv_view_ = &view_matrix_;
    panel.focused_element_ = &focused_element_;
    panel.onBoundsChange_();
}

UIRoot::~UIRoot()
{
    InputController::cancelMouseMotionListener(mouse_motion_listener_id_);
    InputController::cancelMouseButtonAnyListener(mouse_button_any_listener_id_);
    InputController::cancelKeyAllListener(key_all_listener_id_);
    InputController::cancelCharInputListener(char_input_listener_id_);
    getEngine().getWindow()->cancelContextResizeListener(context_resize_listener_id_);
}

void UIRoot::draw()
{
    panel.draw();
}

void UIRoot::clearFocus()
{
    panel.setFocused();
}

UIElement* UIRoot::getFocus()
{
    return focused_element_;
}

UIElement* UIRoot::getElementUnderMouse()
{
    return under_mouse_;
}

void UIRoot::setInputEnabled(bool enabled)
{
    input_handlers_active_ = enabled;
}

bool UIRoot::isInputEnabled() const
{
    return input_handlers_active_;
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

    //PBJ_LOG(VInfo) << "under mouse: " << under_mouse << PBJ_LOG_END;

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

            if (!under_mouse)
                clearFocus();
        }
        else
        {
            // mouse up event for one of first 3 buttons

            if (*down_over)
            {
                if (*down_over != under_mouse)
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
