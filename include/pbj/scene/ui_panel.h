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
/// \file   pbj/scene/ui_panel.h
/// \author Ben Crist

#ifndef PBJ_SCENE_UI_PANEL_H_
#define PBJ_SCENE_UI_PANEL_H_

#include "pbj/scene/ui_element.h"

#include "pbj/engine.h"
#include "pbj/gfx/mesh.h"

#include <memory>
#include <vector>

namespace pbj {
namespace scene {

class UIRoot;
class UIListbox;

struct UIPanelAppearance
{
    bool solid;                 ///< If true, the panel will capture mouse/keyboard events and render its edges

    color4 background_color;    ///< The color of the inside of the panel
    color4 border_color;        ///< The color of the border of the panel
    color4 margin_color;        ///< The color of the margins of the panel

    F32 margin_left;            ///< The distance between the left edge of the panel and the middle of the left border.
    F32 margin_right;
    F32 margin_top;
    F32 margin_bottom;

    F32 border_width_left;      ///< The distance between the center of the border and the edge of the border. (should actually be called 'half_width')
    F32 border_width_right;
    F32 border_width_top;
    F32 border_width_bottom;

    UIPanelAppearance();
};

class UIPanel : public UIElement
{
    friend class UIRoot;
    friend class UIListbox;

public:
    UIPanel();
    virtual ~UIPanel();

    void setAppearance(const UIPanelAppearance& appearance);
    const UIPanelAppearance& getAppearance() const;

    void setScale(const vec2& scale);
    const vec2& getScale() const;

    void addElement(std::unique_ptr<UIElement>&& element);
    
    virtual UIElement* getElementAt(const ivec2& screen_position);

    virtual void draw();

private:
    virtual void onBoundsChange_();

    void calculateTransform_();

    std::vector<std::unique_ptr<UIElement> > elements_;

    vec2 scale_;

    UIPanelAppearance appearance_;
    const gfx::Mesh& panel_mesh_;
    vec2 border_bounds_[4];
    mat4 panel_transform_;
    bool panel_transform_valid_;

    static const size_t u_transform_ = 0;
    static const size_t u_border_bounds_ = 1;
    static const size_t u_border_color_ = 2;
    static const size_t u_background_color_ = 3;
    static const size_t u_outside_color_ = 4;

    gfx::UniformConfig uniforms_[5];
    gfx::BatcherTask btask_;

    mat4 view_matrix_;
    mat4 inv_view_matrix_;
    U32 combined_order_index_offset_;

    UIPanel(const UIPanel&);
    void operator=(const UIPanel&);
};

} // namespace pbj::scene
} // namespace pbj

#endif