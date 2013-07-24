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
/// \file   pbj/scene/ui_button.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::scene::UIButton functions.

#include "pbj/scene/ui_button.h"

#include "pbj/_math.h"

namespace pbj {
namespace scene {

UIButton::UIButton()
    : state_(SActive)
{
    label_.setAlign(UILabel::AlignCenter);
    label_.set

    for (int state = 0; state < 4; ++state)
    {
        colors_[CBackground][state] = color4(1.0f, 1.0f, 1.0f, 0.5f);
        colors_[CBorder][state] = color4(1.0f, 1.0f, 1.0f, 1.0f);
        colors_[CText][state] = color4(1.0f, 1.0f, 1.0f, 1.0f);

        border_widths_[state] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        margins_[state] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    setState(SNormal);
}

UIButton::~UIButton()
{
}

void UIButton::setText(const std::string& text)
{
    label_.setText(text);
}

const std::string& UIButton::getText() const
{
    return label_.getText();
}

void UIButton::onMouseIn(const ivec2& position)
{
}
	
	
void UIButton::onMouseOut(const ivec2& position)
{
}
	
void UIButton::onMouseDown(F32 button)
{
}
	
void UIButton::onMouseUp(F32 button)
{
}
	
void UIButton::onMouseClick(F32 button)
{
}
	
void UIButton::onMouseDblClick(F32 button)
{
}

void UIButton::onKeyDown(F32 keycode)
{
}
	
void UIButton::onKeyUp(F32 keycode)
{
}
	
void UIButton::onKeyPressed(F32 keycode)
{
}
	
void UIButton::onCharacter(F32 codepoint)
{
}

void UIButton::onBoundsChange()
{
}


} // namespace pbj::scene
} // namespace pbj
