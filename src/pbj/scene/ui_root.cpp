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
/// \file   pbj/scene/ui_root.cpp
/// \author Ben Crist

#include "pbj/scene/ui_root.h"

#include "pbj/engine.h"
#include "pbj/window.h"
#include "pbj/input_controller.h"

#include <cassert>

namespace pbj {
namespace scene {

UIRoot::UIRoot()
{
    handle_.associate(this);

    be::Handle<UIRoot> handle = handle_;

    InputController::registerMouseMotionListener(
        [=](F64 x, F64 y)
        {
            UIRoot* root = handle.get();
            if (root)
                root->onMouseMove(ivec2(x, y));
        }
    );

    InputController::registerMouseButtonAnyListener(
        [=](I32 button, I32 action, I32 mods)
        {
            UIRoot* root = handle.get();
            if (root)
                root->onMouseButton(button, action == GLFW_PRESS);
        }
    );

    Window* window = getEngine().getWindow();

    assert(window);

    window->registerContextResizeListener(
        [=](I32 width, I32 height)
        {
            UIRoot* root = handle.get();
            if (root)
            {
                root->projection_ = glm::ortho(0, width, height, 0);
            }
        }
    );

    projection_ = glm::ortho(0, window->getContextSize().x, window->getContextSize().y, 0);
}

UIRoot::~UIRoot()
{
}

void UIRoot::draw()
{
    // TODO: draw root panel
    panel.draw(projection_);
}

void UIRoot::onMouseMove(const ivec2& position)
{
    UIElement* under_mouse = panel.getElementAt(position);

    if (under_mouse != under_mouse_)
    {
        if (under_mouse_)
            under_mouse_->onMouseOut(position);
    }
}

} // namespace pbj::scene
} // namespace pbj
