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
/// \file   pbj/scene/ui_button.h
/// \author Benjamin Crist
///
/// \brief  pbj::scene::UIButton class header.

#ifndef PBJ_SCENE_UI_BUTTON_H_
#define PBJ_SCENE_UI_BUTTON_H_

#include "pbj/scene/ui_label.h"
#include "pbj/gfx/mesh.h"

#include <functional>

namespace pbj {
namespace scene {

struct UIButtonStateConfig
{
    Id button_state;            ///< The button state under which these parameters apply
    
    std::function<void()> click_callback;

    be::ConstHandle<gfx::TextureFont> font;
    vec2 text_scale;

    color4 background_color;    ///< The color of the inside of the button
    color4 border_color;        ///< The color of the border of the button
    color4 text_color;          ///< The color of the button text
    color4 margin_color;        ///< The color of the margins of the button

    F32 margin_left;            ///< The distance between the left edge of the button and the middle of the left border.
    F32 margin_right;
    F32 margin_top;
    F32 margin_bottom;

    F32 border_width_left;      ///< The distance between the center of the border and the edge of the border. (should actually be called 'half_width')
    F32 border_width_right;
    F32 border_width_top;
    F32 border_width_bottom;

    UIButtonStateConfig();
};

///////////////////////////////////////////////////////////////////////////////
/// \brief  Pushbutton UI element.
class UIButton : public UIElement
{
public:
    UIButton();
    virtual ~UIButton();

    void setText(const std::string& text);
    const std::string& getText() const;

    void setStateConfig(const UIButtonStateConfig& config);
    const UIButtonStateConfig& getStateConfig(const Id& state) const;

    void setNormalState(const Id& state);
    const Id& getNormalState() const;

    void setHoveredState(const Id& state);
    const Id& getHoveredState() const;

    void setActiveState(const Id& state);
    const Id& getActiveState() const;

    void setFocusedState(const Id& state);
    const Id& getFocusedState() const;

    void setFocusedHoveredState(const Id& state);
    const Id& getFocusedHoveredState() const;

    void setFocusedActiveState(const Id& state);
    const Id& getFocusedActiveState() const;

    void setDisabledState(const Id& state);
    const Id& getDisabledState() const;

    virtual void draw();

    void setDisabled(bool disabled);
    bool isDisabled() const;

    bool isActive() const;
    bool isHovered() const;

    void onMouseIn(const ivec2& position);
    void onMouseOut(const ivec2& position);
    void onMouseDown(I32 button);
    void onMouseUp(I32 button);
    void onMouseClick(I32 button);
    void onKeyUp(I32 keycode, I32 modifiers);
    void onKeyPressed(I32 keycode, I32 modifiers);

private:
    virtual void onBoundsChange_();

    virtual void onFocusChange_();

    const Id& getCurrentState_();
    void setState_(const Id& state);
    void calculateTransforms_();
    UIButtonStateConfig* getStateConfig_(const Id& id);

    static const UIButtonStateConfig& getDefaultStateConfig_();

    UILabel label_;

    const gfx::Mesh& btn_mesh_;
    GLuint shader_program_id_;
    GLint transform_uniform_location_;
    GLint border_bounds_uniform_location_;
    GLint border_color_uniform_location_;
    GLint background_color_uniform_location_;
    GLint outside_color_uniform_location_;
    mat4 btn_transform_;
    bool btn_transform_valid_;

    std::vector<UIButtonStateConfig> state_configs_;

    Id normal_state_;
    Id hovered_state_;
    Id active_state_;
    Id disabled_state_;
    Id focused_state_;
    Id focused_hovered_state_;
    Id focused_active_state_;

    bool disabled_;
    bool active_;
    bool kbd_active_;
    bool hovered_;

    UIButton(const UIButton&);
    void operator=(const UIButton&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
