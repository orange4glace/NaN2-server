// breakable.h
#ifndef BREAKABLE_H_
#define BREAKABLE_H_

#include "entity.h"
#include "collider_interface.h"

namespace nan2 {

  class Breakable : public Entity, public ColliderInterface {

  private:

  public:

    Breakable(World* world, const Vector2& position);

    virtual void Destroy();

  };
  
}

#endif