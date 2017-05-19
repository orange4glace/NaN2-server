// dropped_item.h
#ifndef DROPPED_ITEM_H_
#define DROPPED_ITEM_H_

#include "entity.h"
#include "collider_interface.h"

namespace nan2 {

  class DroppedItem : public Entity, public ColliderInterface {

  private:

    const static Vector2 SIZE_;

    Vector2 position_;

  public:

    DroppedItem(World* world, const Vector2& position);

    virtual const AABB collider() const;
    
    virtual void Destroy();

  };
  
}

#endif