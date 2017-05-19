// dropped_item.cpp
#include "entity/dropped_item.h"

namespace nan2 {

  const Vector2 DroppedItem::SIZE_ = Vector2(15, 15);

  DroppedItem::DroppedItem(World* world, const Vector2& position) :
    Entity(world) {
    position_= position;
  }

  const AABB DroppedItem::collider() const {
    return AABB(Rect(position_, DroppedItem::SIZE_));
  }

  void DroppedItem::Destroy() {

  }

}