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
/// \file   pbj/editor.h
/// \author Benjamin Crist
///
/// \brief  pbj::Editor class header.

#ifndef PBJ_EDITOR_H_
#define PBJ_EDITOR_H_

#include "be/id.h"
#include "pbj/_pbj.h"
#include "pbj/window.h"
#include "pbj/engine.h"
#include "pbj/gfx/built_ins.h"
#include "pbj/gfx/batcher.h"

#include "pbj/scene/scene.h"
#include "pbj/scene/ui_button.h"

#include <memory>

namespace pbj {

class Editor
{
public:
    Editor();
    ~Editor();

    void initUI();

    void run();

private:
    void onContextResized_(I32 width, I32 height);


    Engine& engine_;
    gfx::Batcher& batcher_;
    const gfx::BuiltIns& builtins_;
    Window& window_;

    scene::Scene scene_;

    std::unordered_map<Id, scene::UIElement*> ui_elements_;

    scene::UIElement* last_created_focusable_element_;
    scene::UIButtonStateConfig bsc_[7];
    std::unique_ptr<scene::UIElement> newButton_(const Id& id,
        const std::string& text,
        const vec2& position, const vec2& dimensions,
        const std::function<void()>& callback);

    Editor(const Editor&);
    void operator=(const Editor&);
};

} // namespace pbj

#endif
