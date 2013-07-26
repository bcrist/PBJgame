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
/// \file   pbj/scene/ui_container.h
/// \author Ben Crist

#ifndef PBJ_SCENE_UI_CONTAINER_H_
#define PBJ_SCENE_UI_CONTAINER_H_

#include "pbj/scene/ui_element.h"

#include <functional>
#include <memory>
#include <vector>

namespace pbj {
namespace scene {

class UIContainer : public UIElement
{
public:
    UIContainer();
    virtual ~UIContainer();

    virtual void addElement(std::unique_ptr<UIElement>&& element);
    
    virtual UIElement* getElementAt(const ivec2& position);

    virtual void draw(const mat4& view_projection);

protected:
    std::vector<std::unique_ptr<UIElement> > elements_;

private:
    ivec2 scale_;

    UIContainer(const UIContainer&);
    void operator=(const UIContainer&);
};

} // namespace pbj::scene
} // namespace pbj

#endif