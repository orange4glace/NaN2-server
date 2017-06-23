// obtainable.h
#ifndef ObtainABLE_H_
#define ObtainABLE_H_

#include "entity.h"
#include "collider_interface.h"

namespace nan2 {

  class Obtainable : public Entity, public ColliderInterface {

  private:

    const static Vector2 SIZE_;

    Entity* content_;

  public:

    Obtainable(World* world, entity_type type, Entity* content, const Vector2& position);
    ~Obtainable();

    Entity* content();
    virtual const AABB collider() const;

  };
  
}

#endif