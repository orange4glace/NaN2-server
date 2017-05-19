// root_box.cpp
#include "entity/root_box.h"

namespace nan2 {

  const Vector2 RootBox::SIZE_ = Vector2(15, 15);

  RootBox::RootBox(World* world, const Vector2& position) :
    Entity(world) {
    position_= position;
  }

  const AABB RootBox::collider() const {
    return AABB(Rect(position_, RootBox::SIZE_));
  }

  void RootBox::Destroy() {

  }

}