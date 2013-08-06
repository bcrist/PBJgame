///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_listbox.cpp
/// \author Ben Crist

#include "pbj/scene/ui_listbox.h"

namespace pbj {
namespace scene {

UIListbox::UIListbox()
    : bounds_valid_(false)
{
    scroll_up = new UIButton();
    panel.addElement(std::unique_ptr<UIElement>(scroll_up));

    UIElement* prev = scroll_up;

    for (int i = 0; i < 16; ++i)
    {
        UIButton* el = list_buttons[i] = new UIButton();
        panel.addElement(std::unique_ptr<UIElement>(el));

        prev->setNextFocusElement(el);
        prev = el;

        el->setActiveState(Id("__selected__"));
        el->setFocusedActiveState(Id("__focused_selected__"));
    }
    scroll_down = new UIButton();
    panel.addElement(std::unique_ptr<UIElement>(scroll_down));

    prev->setNextFocusElement(scroll_down);

}

UIListbox::~UIListbox()
{
}

UIElement* UIListbox::getElementAt(const ivec2& screen_position)
{
    return panel.getElementAt(screen_position);
}

void UIListbox::draw()
{
    if (!isVisible() || !model)
        return;

    if (!bounds_valid_ || model->isDirty())
        calculateBounds_();

    panel.draw();
}

void UIListbox::onUpClick()
{
    if (first_visible_index_ > 0)
        first_visible_index_--;

    bounds_valid_ = false;
}

void UIListbox::onClick(U32 index)
{
    index += first_visible_index_;

    if (model && index < model->size())
        selected_index_ = index;

    bounds_valid_ = false;
}

void UIListbox::onDownClick()
{
    if (model && first_visible_index_ + 16 < model->size())
        first_visible_index_++;

    bounds_valid_ = false;
}

void UIListbox::onBoundsChange_()
{
    panel.order_index_offset_ = order_index_offset_;
    panel.scissor_ = scissor_;
    panel.projection_ = projection_;
    panel.view_ = view_;
    panel.inv_view_ = inv_view_;
    panel.focused_element_ = focused_element_;
    bounds_valid_ = false;
}

void UIListbox::calculateBounds_()
{
    vec2 top_left(getPosition());
    vec2 size(getDimensions());

    int btn_size = int((size.y - 21.0f) / 18.0f);
    size.y = float(btn_size * 18 + 21);

    panel.setPosition(top_left);
    panel.setDimensions(size);

    top_left = vec2(2.0f, 2.0f);
    size = vec2(size.x - 4.0f, btn_size);
    
    scroll_up->setPosition(top_left);
    scroll_up->setDimensions(size);

    top_left.y += size.y + 1;

    Id selected("__selected__");
    Id normal("__normal__");
    Id focused_selected("__focused_selected__");
    Id focused_normal("__focused_normal__");

    for (int i = 0; i < 16; ++i)
    {
        if (model && first_visible_index_ + i < model->size())
        {
            list_buttons[i]->setDisabled(false);
            list_buttons[i]->setText((*model)[first_visible_index_ + i]);

            if (first_visible_index_ + i == selected_index_)
            {
                list_buttons[i]->setNormalState(selected);
                list_buttons[i]->setHoveredState(selected);
                list_buttons[i]->setFocusedState(focused_selected);
                list_buttons[i]->setFocusedHoveredState(focused_selected);
            }
            else
            {
                list_buttons[i]->setNormalState(normal);
                list_buttons[i]->setHoveredState(normal);
                list_buttons[i]->setFocusedState(normal);
                list_buttons[i]->setFocusedHoveredState(normal);
            }
        }
        else if (first_visible_index_ + i >= model->size())
        {
            list_buttons[i]->setDisabled(true);
        }

        list_buttons[i]->setPosition(top_left);
        list_buttons[i]->setDimensions(size);
        top_left.y += size.y + 1;
    }

    scroll_down->setPosition(top_left);
    scroll_down->setDimensions(size);
}

} // namespace pbj::scene
} // namespace pbj
