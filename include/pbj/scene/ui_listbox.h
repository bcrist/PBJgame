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
/// \file   pbj/scene/ui_listbox.h
/// \author Ben Crist

#ifndef PBJ_SCENE_UI_LISTBOX_H_
#define PBJ_SCENE_UI_LISTBOX_H_

#include "pbj/scene/ui_panel.h"
#include "pbj/scene/ui_button.h"
#include "pbj/scene/ui_label.h"

namespace pbj {
namespace scene {

class UIListboxModel
{
public:
    virtual ~UIListboxModel() {}

    virtual std::string operator[](size_t index) = 0;
    virtual size_t size() const = 0;

    virtual bool isDirty() = 0;  ///< returns true when listbox data has changed since the last call to isDirty().
};

class UIListbox : public UIElement
{
    friend class UIRoot;

public:
    UIListbox();
    virtual ~UIListbox();

    UIPanel panel;
    UIButton* scroll_up;
    UIButton* scroll_down;
    UIButton* list_buttons[16];
    
    std::unique_ptr<UIListboxModel> model;

    virtual UIElement* getElementAt(const ivec2& screen_position);

    I32 getSelectedIndex() const;

    virtual void draw();

    void onUpClick();
    void onClick(U32 index);
    void onDownClick();

private:
    virtual void onBoundsChange_();

    void calculateBounds_();

    bool bounds_valid_;

    size_t first_visible_index_;
    size_t selected_index_;

    UIListbox(const UIListbox&);
    void operator=(const UIListbox&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
