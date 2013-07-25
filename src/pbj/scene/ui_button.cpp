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
#include "pbj/gfx/shader_program.h"

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
      btn_mesh_(getEngine().getBuiltIns().getMesh(Id("Mesh.std_quad"))),
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

    const gfx::BuiltIns& builtins = getEngine().getBuiltIns();

    shader_program_id_ = builtins.getProgram(Id("ShaderProgram.UIButton")).getGlId();
    transform_uniform_location_ = glGetUniformLocation(shader_program_id_, "transform");
    border_bounds_uniform_location_ = glGetUniformLocation(shader_program_id_, "border_bounds");
    border_color_uniform_location_ = glGetUniformLocation(shader_program_id_, "border_color");
    background_color_uniform_location_ = glGetUniformLocation(shader_program_id_, "background_color");
    outside_color_uniform_location_ = glGetUniformLocation(shader_program_id_, "outside_color");

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

void UIButton::setStateConfig(const UIButtonStateConfig& config)
{
    for (UIButtonStateConfig& cfg : state_configs_)
    {
        if (cfg.button_state == config.button_state)
        {
            cfg = config;
            setState_(getCurrentState_());
            return;
        }
    }
    state_configs_.push_back(config);
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

        auto cfg = getStateConfig_(getCurrentState_());

        if (cfg)
        {
            const ivec2& dims(getDimensions());
            vec2 inv_scale(1.0f / dims.x, 1.0f / dims.y);
            vec2 border_bounds[4];

            border_bounds[0] = inv_scale * vec2(cfg->margin_left + cfg->border_width_left, cfg->margin_top + cfg->border_width_top);
            border_bounds[1] = vec2(1, 1) - inv_scale * vec2(cfg->margin_right + cfg->border_width_right, cfg->margin_bottom + cfg->border_width_bottom);

            border_bounds[2] = inv_scale * vec2(cfg->margin_left - cfg->border_width_left, cfg->margin_top - cfg->border_width_top);
            border_bounds[3] = vec2(1, 1) - inv_scale * vec2(cfg->margin_right - cfg->border_width_right, cfg->margin_bottom - cfg->border_width_bottom);

            glUseProgram(shader_program_id_);   // set up shader program

            glUniformMatrix4fv(transform_uniform_location_, 1, false, glm::value_ptr(view_projection * btn_transform_));
            glUniform2fv(border_bounds_uniform_location_, 4, glm::value_ptr(border_bounds[0]));
            glUniform4fv(border_color_uniform_location_, 1, glm::value_ptr(cfg->border_color));
            glUniform4fv(background_color_uniform_location_, 1, glm::value_ptr(cfg->background_color));
            glUniform4fv(outside_color_uniform_location_, 1, glm::value_ptr(cfg->margin_color));

            glBindVertexArray(btn_mesh_.getVaoId()); // bind VAO
            glDrawElements(GL_TRIANGLES, btn_mesh_.getIndexCount(), btn_mesh_.getIndexType(), 0);
            glBindVertexArray(0);       // unbind VAO
            glUseProgram(0);            // unbind shader program

            label_.draw(view_projection);
        }
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
    if (kbd_active_ && ((modifiers & (GLFW_MOD_ALT | GLFW_MOD_CONTROL | GLFW_MOD_SHIFT | GLFW_MOD_SUPER)) == 0))
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
    if (isFocused() && ((modifiers & (GLFW_MOD_ALT | GLFW_MOD_CONTROL | GLFW_MOD_SHIFT | GLFW_MOD_SUPER)) == 0))
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
    btn_transform_valid_ = false;
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
    vec3 scale(getDimensions(), 1);
    vec3 translate(getPosition(), 0);

    btn_transform_ = glm::scale(glm::translate(mat4(), translate), scale);
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

const UIButtonStateConfig& UIButton::getDefaultStateConfig_()
{
    static UIButtonStateConfig cfg;
    return cfg;
}


} // namespace pbj::scene
} // namespace pbj
