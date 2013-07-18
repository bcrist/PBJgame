// Copyright (c) 2013 PBJ Productions
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
/// \file   pbj/ui_image.h
/// \author Josh Douglas
///
/// \brief  pbj::UIImage class header.

#ifndef PBJ_UI_IMAGE_H_
#define PBJ_UI_IMAGE_H_

#include "pbj/ui_element.h"
#include "pbj/scene/texture.h"
#include "be/const_handle.h"

namespace pbj {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Text Image UI element.
class UIImage : public UIElement
{
public:
	UIImage();
	virtual ~UIImage();
	void setTexture(const be::ConstHandle<scene::Texture> &texture);
    const be::ConstHandle<scene::Texture> &getTexture() const;

private:
	be::ConstHandle<scene::Texture> texture_;
};

}// namespace pbj 

#endif
