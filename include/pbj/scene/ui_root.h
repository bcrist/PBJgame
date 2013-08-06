///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_root.h
/// \author Ben Crist

#ifndef PBJ_SCENE_UI_ROOT_H_
#define PBJ_SCENE_UI_ROOT_H_

#include "pbj/scene/ui_panel.h"

#include <functional>
#include <vector>

#include "be/source_handle.h"

#include "pbj/_gl.h"
#include "pbj/_pbj.h"

namespace pbj {
namespace scene {

class UIRoot
{
public:
    UIRoot();
    ~UIRoot();

    UIPanel panel;

    void draw();

    void clearFocus();

    UIElement* getFocus();
    UIElement* getElementUnderMouse();

    void setInputEnabled(bool enabled);
    bool isInputEnabled() const;

private:
    void onMouseMove(const ivec2& position);
    void onMouseButton(I32 button, bool down);
    void onKey(I32 keycode, I32 action, I32 modifiers);
    void onCharacter(I32 codepoint);

    be::SourceHandle<UIRoot> handle_;

    ivec2 mouse_position_;
    UIElement* under_mouse_;
    UIElement* focused_element_;
    UIElement* button1_down_over_;
    UIElement* button2_down_over_;
    UIElement* button3_down_over_;

    U32 order_index_offset_;
    mat4 projection_matrix_;
    mat4 view_matrix_;

    U32 mouse_motion_listener_id_;
    U32 mouse_button_any_listener_id_;
    U32 key_all_listener_id_;
    U32 char_input_listener_id_;
    U32 context_resize_listener_id_;

    bool input_handlers_active_;

    UIRoot(const UIRoot&);
    void operator=(const UIRoot&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
