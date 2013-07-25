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
/// \file   pbj/scene/ui_button.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::scene::UIButton functions.

#include "pbj/scene/ui_button.h"

#include "pbj/engine.h"
#include "pbj/gfx/built_ins.h"

#include "pbj/_math.h"

namespace pbj {
namespace scene {

UIButtonStateConfig::UIButtonStateConfig()
    : button_state("__unknown__"),
      text_scale(2.0f, 2.0f),
      background_color(0.5f, 0.5f, 0.5f, 0.5f),
      border_color(1.0f, 1.0f, 1.0f, 1.0f),
      text_color(1.0f, 1.0f, 1.0f, 1.0f),
      margin_color(0.0f, 0.0f, 0.0f, 0.0f),
      margin_left(1.0f),
      margin_right(1.0f),
      margin_top(1.0f),
      margin_bottom(1.0f),
      border_width_left(1.0f),
      border_width_right(1.0f),
      border_width_top(1.0f),
      border_width_bottom(1.0f)
{
}

UIButton::UIButton()
    : btn_transform_valid_(false),
      normal_state_("__normal__"),
      hovered_state_("__hovered__"),
      active_state_("__active__"),
      disabled_state_("__disabled__"),
      focused_state_("__hovered__"),
      focused_hovered_state_("__hovered__"),
      focused_active_state_("__active__"),
      disabled_(false),
      active_(false),
      hovered_(false)
{
    label_.setAlign(UILabel::AlignCenter);

    setState_(getCurrentState_());
}

UIButton::~UIButton()
{
}

void UIButton::setText(const std::string& text)
{
    label_.setText(text);
}

const std::string& UIButton::getText() const
{
    return label_.getText();
}

void UIButton::setStateConfig(const Id& state, const UIButtonStateConfig& config)
{
    for (UIButtonStateConfig& cfg : state_configs_)
    {
        if (cfg.button_state == state)
        {
            cfg = config;
            setState_(getCurrentState_());
            return;
        }
    }
}

const UIButtonStateConfig& UIButton::getStateConfig(const Id& state) const
{
    for (const UIButtonStateConfig& cfg : state_configs_)
    {
        if (cfg.button_state == state)
            return cfg;
    }

    return getDefaultStateConfig_();
}

void UIButton::setNormalState(const Id& state)
{
    normal_state_ = state;
}
const Id& UIButton::getNormalState() const
{
    return normal_state_;
}

void UIButton::setHoveredState(const Id& state)
{
    hovered_state_ = state;
}
const Id& UIButton::getHoveredState() const
{
    return hovered_state_;
}

void UIButton::setActiveState(const Id& state)
{
    active_state_ = state;
}
const Id& UIButton::getActiveState() const
{
    return active_state_;
}

void UIButton::setFocusedState(const Id& state)
{
    focused_state_ = state;
}
const Id& UIButton::getFocusedState() const
{
    return focused_state_;
}

void UIButton::setFocusedHoveredState(const Id& state)
{
    focused_hovered_state_ = state;
}
const Id& UIButton::getFocusedHoveredState() const
{
    return focused_hovered_state_;
}

void UIButton::setFocusedActiveState(const Id& state)
{
    focused_active_state_ = state;
}
const Id& UIButton::getFocusedActiveState() const
{
    return focused_active_state_;
}

void UIButton::setDisabledState(const Id& state)
{
    disabled_state_ = state;
}
const Id& UIButton::getDisabledState() const
{
    return disabled_state_;
}

void UIButton::draw(const mat4& view_projection)
{
    // TODO
    if (isVisible())
    {
        if (!btn_transform_valid_)
            calculateTransforms_();

        getEngine().getBuiltIns().getMesh(
    }
}

void UIButton::setDisabled(bool disabled)
{
    disabled_ = disabled;
}

bool UIButton::isDisabled() const
{
    return disabled_;
}

bool UIButton::isActive() const
{
    return active_;
}

bool UIButton::isHovered() const
{
    return hovered_;
}

void UIButton::onMouseIn(const ivec2& position)
{
    hovered_ = true;
    setState_(getCurrentState_());}	
	
void UIButton::onMouseOut(const ivec2& position)
{
    hovered_ = false;
    setState_(getCurrentState_());
}
	
void UIButton::onMouseDown(I32 button)
{
    UIElement::onMouseDown(button);
    if (button == GLFW_MOUSE_BUTTON_1)
        active_ = true;
}
	
void UIButton::onMouseUp(I32 button)
{
    if (button == GLFW_MOUSE_BUTTON_1)
        active_ = false;
}
	
void UIButton::onMouseClick(I32 button)
{
    if (!disabled_ && button == GLFW_MOUSE_BUTTON_1)
    {
        auto cfg = getStateConfig_(getCurrentState_());

        if (cfg && cfg->click_callback)
            cfg->click_callback();
    }

}
	
void UIButton::onKeyUp(I32 keycode, I32 modifiers)
{
    if (kbd_active_ && modifiers & (GLFW_MOD_ALT | GLFW_MOD_CONTROL | GLFW_MOD_SHIFT | GLFW_MOD_SUPER) == 0)
    {
        if (keycode == GLFW_KEY_SPACE || keycode == GLFW_KEY_ENTER || keycode == GLFW_KEY_KP_ENTER)
        {
            onMouseClick(GLFW_MOUSE_BUTTON_1);
            kbd_active_ = false;
            setState_(getCurrentState_());
        }
    }
}
	
void UIButton::onKeyPressed(I32 keycode, I32 modifiers)
{
    UIElement::onKeyPressed(keycode, modifiers);
    if (isFocused() && modifiers & (GLFW_MOD_ALT | GLFW_MOD_CONTROL | GLFW_MOD_SHIFT | GLFW_MOD_SUPER) == 0)
    {
        if (keycode == GLFW_KEY_SPACE || keycode == GLFW_KEY_ENTER || keycode == GLFW_KEY_KP_ENTER)
        {
            kbd_active_ = true;
            setState_(getCurrentState_());
        }
    }
}
	
void UIButton::onBoundsChange_()
{
    calculateTransforms_();
}

void UIButton::onFocusGained_()
{
    setState_(getCurrentState_());
}

void UIButton::onFocusLost_()
{
    kbd_active_ = false;
    setState_(getCurrentState_());
}

const Id& UIButton::getCurrentState_()
{
    if (disabled_)
        return disabled_state_;

    if (isFocused())
    {
        if ((hovered_ && active_) || kbd_active_)
            return focused_active_state_;

        if (hovered_)
            return focused_hovered_state_;

        return focused_state_;
    }
    else
    {
        if (hovered_)
        {
            if (active_)
                return active_state_;

            return hovered_state_;
        }

        return normal_state_; 
    }
}

void UIButton::setState_(const Id& state)
{
    auto cfg = getStateConfig(state);

    label_.setFont(cfg.font);
    label_.setTextScale(cfg.text_scale);
    label_.setTextColor(cfg.text_color);

    label_.setPosition(getPosition() + ivec2(I32(cfg.margin_left + cfg.border_width_left), I32(cfg.margin_top + cfg.border_width_top)));
    label_.setDimensions(getDimensions() - ivec2(I32(cfg.margin_left + cfg.border_width_left + cfg.border_width_right + cfg.margin_right),
                                                 I32(cfg.margin_top + cfg.border_width_top + cfg.border_width_bottom + cfg.margin_bottom)));
}

void UIButton::calculateTransforms_()
{
    btn_transform_ = glm::translate(mat4(), vec3(getPosition(), 0));
    setState_(getCurrentState_());
    btn_transform_valid_ = true;
}

UIButtonStateConfig* UIButton::getStateConfig_(const Id& id)
{
    for(UIButtonStateConfig& cfg : state_configs_)
    {
        if (cfg.button_state == id)
            return &cfg;
    }

    return nullptr;
}

const UIButtonStateConfig& getDefaultStateConfig_()
{
    static UIButtonStateConfig cfg;
    return cfg;
}


} // namespace pbj::scene
} // namespace pbj
