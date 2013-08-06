///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene.h
/// \author Benjamin Crist
///
/// \brief  pbj::scene::Scene class header.

#ifndef PBJ_SCENE_SCENE_H_
#define PBJ_SCENE_SCENE_H_

#include "pbj/_pbj.h"
#include "pbj/scene/ui_root.h"

namespace pbj {
namespace scene {

class Scene
{
public:
    Scene();

    UIRoot ui;

private:

    Scene(const Scene&);
    void operator=(const Scene&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
