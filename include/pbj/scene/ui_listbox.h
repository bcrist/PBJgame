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
