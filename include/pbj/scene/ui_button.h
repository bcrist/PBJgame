///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_button.h
/// \author Benjamin Crist
///
/// \brief  pbj::scene::UIButton class header.

#ifndef PBJ_SCENE_UI_BUTTON_H_
#define PBJ_SCENE_UI_BUTTON_H_

#include "pbj/scene/ui_label.h"
#include "pbj/gfx/mesh.h"
#include "pbj/gfx/batcher.h"

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

    virtual bool isFocusable() const;

    virtual void onMouseIn(const ivec2& position);
    virtual void onMouseOut(const ivec2& position);
    virtual void onMouseDown(I32 button);
    virtual void onMouseUp(I32 button);
    virtual void onMouseClick(I32 button);
    virtual void onKeyUp(I32 keycode, I32 modifiers);
    virtual void onKeyPressed(I32 keycode, I32 modifiers);

private:
    virtual void onBoundsChange_();
    virtual void onFocusChange_();

    static const UIButtonStateConfig& getDefaultStateConfig_();
    const Id& getCurrentState_();
    void refreshConfig_();
    UIButtonStateConfig* getStateConfig_(const Id& id);

    UILabel label_;

    const gfx::Mesh& btn_mesh_;

    std::vector<UIButtonStateConfig> state_configs_;
    const UIButtonStateConfig* current_config_;
    mat4 btn_transform_;
    vec2 border_bounds_[4];

    static const size_t u_transform_ = 0;
    static const size_t u_border_bounds_ = 1;
    static const size_t u_border_color_ = 2;
    static const size_t u_background_color_ = 3;
    static const size_t u_outside_color_ = 4;

    gfx::UniformConfig uniforms_[5];
    gfx::BatcherTask btask_;

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
