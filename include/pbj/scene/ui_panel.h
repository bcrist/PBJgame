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

#include <memory>
#include <vector>

namespace pbj {
namespace scene {

class UIRoot;

class UIPanel : public UIElement
{
    friend class UIRoot;

public:
    UIPanel();
    virtual ~UIPanel();

    void setScale(const vec2& scale);
    const vec2& getScale() const;

    void addElement(std::unique_ptr<UIElement>&& element);
    
    virtual UIElement* getElementAt(const ivec2& screen_position);

    virtual void draw();

private:
    virtual void onBoundsChange_();

    std::vector<std::unique_ptr<UIElement> > elements_;

    vec2 scale_;

    mat4 view_matrix_;
    mat4 inv_view_matrix_;

    UIPanel(const UIPanel&);
    void operator=(const UIPanel&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
