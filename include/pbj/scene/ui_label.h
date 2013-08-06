///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_label.h
/// \author Benjamin Crist
///
/// \brief  pbj::scene::UILabel class header.

#ifndef PBJ_SCENE_UI_LABEL_H_
#define PBJ_SCENE_UI_LABEL_H_

#include "pbj/scene/ui_element.h"
#include "pbj/gfx/texture_font.h"
#include "pbj/gfx/texture_font_text.h"
#include "be/const_handle.h"

namespace pbj {
namespace scene {

class UIButton;

///////////////////////////////////////////////////////////////////////////////
/// \brief  Text Label UI element.
class UILabel : public UIElement
{
    friend class UIButton;

public:
    enum Align
    {
        AlignLeft,
        AlignCenter,
        AlignRight,
    };

    UILabel();
    virtual ~UILabel();

    virtual UIElement* getElementAt(const ivec2& screen_position);

    void setText(const std::string& text);
    const std::string& getText() const;

    void setTextScale(const vec2& scale);
    const vec2& getTextScale() const;

    void setTextColor(const color4& color);
    const color4& getTextColor() const;

    void setFont(const be::ConstHandle<gfx::TextureFont>& font);
    const be::ConstHandle<gfx::TextureFont>& getFont() const;

    void setAlign(Align align);
    Align getAlign() const;

    virtual void draw();

private:
    virtual void onBoundsChange_();

    void calculateTextTransform_();

    vec2 text_scale_;
    gfx::TextureFontText tf_text_;
    Align align_;

    mat4 text_transform_;
    bool text_transform_valid_;

    UILabel(const UILabel&);
    void operator=(const UILabel&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
