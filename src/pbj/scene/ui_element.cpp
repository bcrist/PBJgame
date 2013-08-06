///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_element.cpp
/// \author Josh Douglas
///
/// \brief  pbj::scene::UIElement class source.

#include "pbj/scene/ui_element.h"

#include "pbj/_gl.h"

#include <vector>

namespace pbj {
namespace scene {


///////////////////////////////////////////////////////////////////////////////
/// \brief  Base constructor for UIElement objects.
///
/// \details An element must be added to a UIRoot's UIPanel or a UIContainer
///         which has been added to it before the element can be drawn or
///         receive input events.
UIElement::UIElement()
    : order_index_offset_(nullptr),
      scissor_(nullptr),
      projection_(nullptr),
      view_(nullptr),
      inv_view_(nullptr),
      focused_element_(nullptr),
      next_focus_(nullptr),
      visible_(true)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Base destructor for UIElement objects.
UIElement::~UIElement()
{
    if (focused_element_ && *focused_element_ == this)
        *focused_element_ = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Determines whether or not the element should be visible.
///
/// \details Invisible elements shall not be drawn, and shall not be focusable
///         or respond to mouse events.  If a focused element turns invisible,
///         it shall become unfocused, and the next element in the focus chain
///         will receive focus.
///
/// \param  visible Whether or not this element should be visible.
void UIElement::setVisible(bool visible)
{
    if (visible_ != visible)
    {
        visible_ = visible;
        onVisibilityChange_();

        if (!visible && focused_element_ && *focused_element_ == this)
        {
            if (next_focus_)
            {
                next_focus_->setFocused();
            }
            else
            {
                *focused_element_ = nullptr;
                onFocusChange_();
            }
        }
    }
}
    
///////////////////////////////////////////////////////////////////////////////
/// \brief  Determines whether the element is visible.
///
/// \return true if the element is visible.
bool UIElement::isVisible() const
{
    return visible_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Gives this element the keyboard focus.
///
/// \details If this element is focusable, it gains the keyboard focus.  If the
///         element is not focusable, the focus chain will be traversed until a
///         focusable element is reached.
void UIElement::setFocused()
{
    if (focused_element_ && *focused_element_ != this)
    {
        UIElement* old_focus = *focused_element_;
        *focused_element_ = this;
        if (old_focus)
            old_focus->onFocusChange_();
        onFocusChange_();
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Determines whether the element is focused.
///
/// \return true if the element has the keyboard focus.
bool UIElement::isFocused() const
{
    return focused_element_ && *focused_element_ == this;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Determines whether this element can be focused at this time.
///
/// \details Elements which are not visible, or are disabled in some way
///         probably should not be focusable.
///
/// \return true if the element can receive keyboard focus now.
bool UIElement::isFocusable() const
{
    return false;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets this element's position in it's parent's coordinate system.
///
/// \param  position The element's position.
void UIElement::setPosition(const vec2& position)
{
    position_ = position;
    onBoundsChange_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the position of this element.
/// 
/// \return The position of this element.
const vec2& UIElement::getPosition() const
{
    return position_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets this element's dimensions.
///
/// \details Together with the element's position and the view and projection
///         matrices, determines the element's bounding box.
///
/// \param  dimensions The element's new size.
void UIElement::setDimensions(const vec2& dimensions)
{
    dimensions_ = dimensions;
    onBoundsChange_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the dimensions of this element.
///
/// \return The element's size.
const vec2& UIElement::getDimensions() const
{
    return dimensions_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Determines if the point at the specified position resides within
///         this element (or one of its children).
///
/// \param  position The screen coordinates of the point in question.
/// \return A pointer to an element at these coordinates, or nullptr if there
///         is no such element.
UIElement* UIElement::getElementAt(const ivec2& screen_position)
{
    if (!inv_view_ || !isVisible())
        return nullptr;

    vec2 pos = vec2(*inv_view_ * vec4(screen_position, 0, 1));

    if (pos.x >= position_.x && pos.x < position_.x + dimensions_.x &&
        pos.y >= position_.y && pos.y < position_.y + dimensions_.y)
    {
        return this;
    }
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the next element in the focus chain.
///
/// \param  element The next element.
void UIElement::setNextFocusElement(UIElement* element)
{
    next_focus_ = element;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the next focusable element in the focus chain.
///
/// \return The next element in the focus chain.
UIElement* UIElement::getNextFocusableElement()
{
    std::vector<UIElement*> visited;
    visited.push_back(this);
    UIElement* cur = next_focus_;

    while (cur && !cur->isFocusable() &&
           std::find(visited.begin(),
                     visited.end(),
                     cur) == visited.end())
    {
        visited.push_back(cur);
        cur = cur->next_focus_;
    }

    if (cur && cur->isFocusable())
        return cur;

    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the previous focusable element in the focus chain.
///
/// \details Since the focus chain is singly-linked, it must be circular to
///         find the previous element.
///
/// \return The previous element in the focus chain.
UIElement* UIElement::getPreviousFocusableElement()
{
    std::vector<UIElement*> visited;
    UIElement* cur = this;
    UIElement* prev_focus = cur->isFocusable() ? cur : nullptr;;

    while (cur->next_focus_ && (
           std::find(visited.begin(),
                     visited.end(),
                     cur->next_focus_) == visited.end()))
    {
        visited.push_back(cur);
        cur = cur->next_focus_;
        if (cur->isFocusable())
            prev_focus = cur;
    }

    if (cur->next_focus_ == this)
        return prev_focus;

    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when the mouse moves into the element.
///
/// \details Technically, called whenever getElementAt() returns this element,
///         but the previous getElementAt() call did not.
///
/// \param  position The current mouse position in screen coordinates.
void UIElement::onMouseIn(const ivec2& screen_position)
{
}
	
///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when the mouse moves while over the element.
/// 
/// \details Technically, called whenever getElementAt() returns this element,
///         regardless of the result of prior calls to getElementAt().
///
///         Additionally, If any of the first 3 mouse buttons are being held
///         down, onMouseMove will be called for the element that was under
///         the mouse at the time the button was pressed.
///
/// \param  position The current mouse position in screen coordinates.
void UIElement::onMouseMove(const ivec2& screen_position)
{
}
	
///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when the mouse moves away from this element.
///
/// \details Technically, called whenever getElementAt() returns something 
///         other than this element, but the previous getElementAt() call
///         returned this.
///
/// \param  position The current mouse position in screen coordinates.
void UIElement::onMouseOut(const ivec2& screen_position)
{
}
	
///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse button is pressed while the mouse is over
///         this element.
///
/// \param  button A GLFW symbolic constant corresponding to the button
///         pressed.
void UIElement::onMouseDown(I32 button)
{
    if (next_focus_)
        setFocused();
}
	
///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse button is released while the mouse is over
///         this element.
///
/// \details Additionally, if the button is one of the first 3 mouse buttons
///         and was pressed over an element, it will recieve the mouseUp
///         event, even if the mouse is no longer over it.
///
/// \param  button A GLFW symbolic constant corresponding to the button
///         released.
void UIElement::onMouseUp(I32 button)
{
}
	
///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse button is pressed and released while the mouse
///         is over this element.
///
/// \details MouseClick events are only fired for the first 3 mouse buttons.
///
/// \param  button A GLFW symbolic constant corresponding to the button
///         clicked.
void UIElement::onMouseClick(I32 button)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse button is clicked on this element twice within
///         350 ms.
///
/// \param  button A GLFW symbolic constant corresponding to the button
///         clicked.
///
/// \bug    Not Yet Implemented!
void UIElement::onMouseDblClick(I32 button)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a key is pressed while this element is focused.
///
/// \param  keycode A GLFW symbolic constant corresponding to the key pressed.
/// \param  modifiers A bitfield representing the state of the modifier keys
///         when the event occurred.
void UIElement::onKeyDown(I32 keycode, I32 modifiers)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a key is released while this element is focused.
///
/// \note   If the element loses focus after a keyDown event but before the
///         keyUp event, it will not receive the event.  Therefore a focus
///         loss should be treated as if all "down" keys were cancelled.
///
/// \param  keycode A GLFW symbolic constant corresponding to the key pressed.
/// \param  modifiers A bitfield representing the state of the modifier keys
///         when the event occurred.
void UIElement::onKeyUp(I32 keycode, I32 modifiers)
{
}
	
///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a key is pressed while this element is focused.
///
/// \details Identical to onKeyDown, but onKeyPressed is not subject to
///         key-repeat events.
///
/// \param  keycode A GLFW symbolic constant corresponding to the key pressed.
/// \param  modifiers A bitfield representing the state of the modifier keys
///         when the event occurred.
void UIElement::onKeyPressed(I32 keycode, I32 modifiers)
{
    if (next_focus_ && keycode == GLFW_KEY_TAB && (0 == (modifiers & (GLFW_MOD_ALT | GLFW_MOD_CONTROL | GLFW_MOD_SUPER))))
    {
        if (modifiers & GLFW_MOD_SHIFT)
        {
            UIElement* prev = getPreviousFocusableElement();
            if (prev)
                prev->setFocused();
        }
        else
        {
            UIElement* next = getNextFocusableElement();
            if (next)
                next->setFocused();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a character is input.
///
/// \note   Character input does not necessarily correspond to key presses in
///         a 1:1 manner.
///
/// \param  codepoint The unicode codepoint of the character entered.
void UIElement::onCharacter(I32 codepoint)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when this element's position or dimensions change.
///
/// \details Can be overridden by derrived classes.
void UIElement::onBoundsChange_()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when this element gains or loses keyboard focus.
///
/// \details Can be overridden by derrived classes.
void UIElement::onFocusChange_()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when this element's visibility changes.
///
/// \details Can be overridden by derrived classes.
void UIElement::onVisibilityChange_()
{
}

} // namespace pbj::scene
} // namespace pbj
