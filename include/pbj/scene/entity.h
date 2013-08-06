///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/entity.h
/// \author Ben Crist

#ifndef PBJ_SCENE_ENTITY_H_
#define PBJ_SCENE_ENTITY_H_

#include "pbj/gfx/mesh.h"
#include "pbj/transform.h"
#include "pbj/_pbj.h"

namespace pbj {
namespace scene {

class Entity
{
public:
    Entity();
    ~Entity();

    Transform transform;

private:

  //  Entity(const Entity&);
  //  void operator=(const Entity&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
