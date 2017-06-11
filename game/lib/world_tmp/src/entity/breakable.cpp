// breakable.cpp
#include "entity/breakable.h"

namespace nan2 {

  const Vector2 Breakable::SIZE_ = Vector2(15, 15);

  Breakable::Breakable(World* world, const Vector2& position) :
    Entity(world, Entity::GROUP_BREAKABLE, Entity::TYPE_BREAKABLE) {
    position_= position;
  }

  const AABB Breakable::collider() const {
    return AABB(Rect(position_, Breakable::SIZE_));
  }

  void Breakable::Destroy() {

  }

}