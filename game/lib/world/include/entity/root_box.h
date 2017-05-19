// root_box.h
#ifndef ROOT_BOX_H_
#define ROOT_BOX_H_

#include "entity.h"
#include "collider_interface.h"

namespace nan2 {

  class RootBox : public Entity, public ColliderInterface {

  private:

    const static Vector2 SIZE_;

  public:

    RootBox(World* world, const Vector2& position);

    virtual const AABB collider() const;

    virtual void Destroy();

  };
  
}

#endif