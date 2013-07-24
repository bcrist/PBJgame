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

struct UIButtonAppearance
{
    Id button_state;            ///< The button state under which these parameters apply
    
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

    bool visible;
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

    void setStateAppearance(const Id& state, const UIButtonAppearance& appearance);
    const UIButtonAppearance& getStateAppearance(const Id& state) const;

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
    const Id& getFocusedHoveredState() const;

    void setDisabledState(const Id& state);
    const Id& getDisabledState() const;

    void setClickCallback(const std::function<void()>& callback);

    virtual void draw(const mat4& view_projection);

    void setDisabled(bool disabled);
    bool isDisabled() const;

    bool isActive() const;
    bool isHovered() const;

    void onMouseIn(const ivec2& position);
    void onMouseOut(const ivec2& position);
    void onMouseDown(F32 button);
    void onMouseUp(F32 button);
    void onMouseClick(F32 button);
    void onMouseDblClick(F32 button);
    void onKeyDown(F32 keycode);
    void onKeyUp(F32 keycode);
    void onKeyPressed(F32 keycode);
    void onCharacter(F32 codepoint);

private:
    virtual void onBoundsChange();

    UIButtonAppearance& getAppearance_(const Id& id);

    UILabel label_;

    std::vector<UIButtonAppearance> state_appearances_;

    Id normal_state_;
    Id hovered_state_;
    Id active_state_;
    Id disabled_state_;
    
    bool disabled_;
    bool active_;
    bool hovered_;
    
    
    
    std::function<void()> click_fn_;

    UIButton(const UIButton&);
    void operator=(const UIButton&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
