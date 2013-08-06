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
      text_scale(1.0f, 1.0f),
      background_color(0.5f, 0.5f, 0.5f, 0.5f),
      border_color(1.0f, 1.0f, 1.0f, 1.0f),
      text_color(1.0f, 1.0f, 1.0f, 1.0f),
      margin_color(0.0f, 0.0f, 0.0f, 0.0f),
      margin_left(0.5f),
      margin_right(0.5f),
      margin_top(0.5f),
      margin_bottom(0.5f),
      border_width_left(0.5f),
      border_width_right(0.5f),
      border_width_top(0.5f),
      border_width_bottom(0.5f)
{
}

UIButton::UIButton()
    : btn_mesh_(getEngine().getBuiltIns().getMesh(Id("Mesh.std_quad"))),
      current_config_(nullptr),
      normal_state_("__normal__"),
      hovered_state_("__hovered__"),
      active_state_("__active__"),
      disabled_state_("__disabled__"),
      focused_state_("__hovered__"),
      focused_hovered_state_("__hovered__"),
      focused_active_state_("__active__"),
      disabled_(false),
      active_(false),
      kbd_active_(false),
      hovered_(false)
{
    label_.setAlign(UILabel::AlignCenter);

    const gfx::BuiltIns& builtins = getEngine().getBuiltIns();

    btask_.program_id = builtins.getProgram(Id("ShaderProgram.UIBox")).getGlId();

    btask_.vao_id = btn_mesh_.getVaoId();
    btask_.n_indices = btn_mesh_.getIndexCount();
    btask_.index_data_type = btn_mesh_.getIndexType();

    btask_.samplers = nullptr;
    btask_.n_samplers = 0;
    btask_.uniforms = uniforms_;
    btask_.n_uniforms = 5;

    uniforms_[u_transform_].location = glGetUniformLocation(btask_.program_id, "transform");
    uniforms_[u_transform_].type = gfx::UniformConfig::UM4f;
    uniforms_[u_transform_].array_size = 1;
    uniforms_[u_transform_].data = glm::value_ptr(btn_transform_);

    uniforms_[u_border_bounds_].location = glGetUniformLocation(btask_.program_id, "border_bounds");
    uniforms_[u_border_bounds_].type = gfx::UniformConfig::U2f;
    uniforms_[u_border_bounds_].array_size = 4;
    uniforms_[u_border_bounds_].data = glm::value_ptr(border_bounds_[0]);

    uniforms_[u_border_color_].location = glGetUniformLocation(btask_.program_id, "border_color");
    uniforms_[u_border_color_].type = gfx::UniformConfig::U4f;
    uniforms_[u_border_color_].array_size = 1;

    uniforms_[u_background_color_].location = glGetUniformLocation(btask_.program_id, "background_color");
    uniforms_[u_background_color_].type = gfx::UniformConfig::U4f;
    uniforms_[u_background_color_].array_size = 1;

    uniforms_[u_outside_color_].location = glGetUniformLocation(btask_.program_id, "outside_color");
    uniforms_[u_outside_color_].type = gfx::UniformConfig::U4f;
    uniforms_[u_outside_color_].array_size = 1;

    btask_.depth_tested = false;
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
            if (&cfg == current_config_)
                current_config_ = nullptr;
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

#pragma region getter/setter functions for normal/active/focused/hovered/etc.

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

#pragma endregion

void UIButton::draw()
{
    if (isVisible() && projection_ && view_)
    {
        refreshConfig_();

        getEngine().getBatcher().submit(btask_);
        label_.draw();
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

bool UIButton::isFocusable() const
{
    return isVisible() && !isDisabled();
}

void UIButton::onMouseIn(const ivec2& position)
{
    hovered_ = true;
}	
	
void UIButton::onMouseOut(const ivec2& position)
{
    hovered_ = false;
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
        }
    }
}
	
void UIButton::onBoundsChange_()
{
    current_config_ = nullptr;
    btask_.order_index = *order_index_offset_;
    btask_.scissor = scissor_;

    label_.order_index_offset_ = order_index_offset_;
    label_.scissor_ = scissor_;
    label_.projection_ = projection_;
    label_.view_ = view_;
    label_.inv_view_ = inv_view_;
    label_.focused_element_ = focused_element_;
    label_.onBoundsChange_();
}

void UIButton::onFocusChange_()
{
    if (!isFocused())
        kbd_active_ = false;
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

void UIButton::refreshConfig_()
{
    const UIButtonStateConfig& cfg = getStateConfig(getCurrentState_());

    if (current_config_ != &cfg)
    {
        if (!(projection_ && view_))
            return;

        current_config_ = &cfg;

        label_.setFont(cfg.font);
        label_.setTextScale(cfg.text_scale);
        label_.setTextColor(cfg.text_color);

        label_.setPosition(getPosition() + vec2(I32(cfg.margin_left + cfg.border_width_left), I32(cfg.margin_top + cfg.border_width_top)));
        label_.setDimensions(getDimensions() - vec2(I32(cfg.margin_left + cfg.border_width_left + cfg.border_width_right + cfg.margin_right),
                                                    I32(cfg.margin_top + cfg.border_width_top + cfg.border_width_bottom + cfg.margin_bottom)));

        uniforms_[u_background_color_].data = glm::value_ptr(cfg.background_color);
        uniforms_[u_border_color_].data = glm::value_ptr(cfg.border_color);
        uniforms_[u_outside_color_].data = glm::value_ptr(cfg.margin_color);

        vec3 scale(getDimensions(), 1);
        vec3 translate(getPosition(), 0);
        btn_transform_ = glm::scale(glm::translate(*projection_ * *view_, translate), scale);

        vec2 inv_scale(1.0f / scale.x, 1.0f / scale.y);

        border_bounds_[0] = inv_scale * vec2(cfg.margin_left + cfg.border_width_left, cfg.margin_top + cfg.border_width_top);
        border_bounds_[1] = vec2(1, 1) - inv_scale * vec2(cfg.margin_right + cfg.border_width_right, cfg.margin_bottom + cfg.border_width_bottom);

        border_bounds_[2] = inv_scale * vec2(cfg.margin_left - cfg.border_width_left, cfg.margin_top - cfg.border_width_top);
        border_bounds_[3] = vec2(1, 1) - inv_scale * vec2(cfg.margin_right - cfg.border_width_right, cfg.margin_bottom - cfg.border_width_bottom);        
    }
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
