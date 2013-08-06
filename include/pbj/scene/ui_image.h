///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_image.h
/// \author Josh Douglas
///
/// \brief  pbj::scene::UIImage class header.

#ifndef PBJ_SCENE_UI_IMAGE_H_
#define PBJ_SCENE_UI_IMAGE_H_

#include "pbj/scene/ui_element.h"
#include "pbj/gfx/texture.h"
#include "be/const_handle.h"

namespace pbj {
namespace scene {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Text Image UI element.
class UIImage : public UIElement
{
public:
	UIImage();
	virtual ~UIImage();
	void setTexture(const be::ConstHandle<gfx::Texture> &texture);
    const be::ConstHandle<gfx::Texture> &getTexture() const;

private:
	be::ConstHandle<gfx::Texture> texture_;
};

} // namespace pbj::scene
} // namespace pbj 

#endif
