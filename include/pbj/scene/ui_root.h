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

private:
    void onMouseMove(const ivec2& position);
    void onMouseButton(I32 button, bool down);
    void onKey(I32 keycode, I32 action, I32 modifiers);
    void onCharacter(I32 codepoint);

    ivec2 mouse_position_;

    UIElement* under_mouse_;

    UIElement* focused_element_;

    UIElement* button1_down_over_;
    UIElement* button2_down_over_;
    UIElement* button3_down_over_;

    be::SourceHandle<UIRoot> handle_;

    mat4 projection_matrix_;
    mat4 view_matrix_;

    UIRoot(const UIRoot&);
    void operator=(const UIRoot&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
