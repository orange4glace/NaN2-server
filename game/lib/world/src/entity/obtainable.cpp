// obtainable.cpp
#include "entity/obtainable.h"

namespace nan2 {

  const Vector2 Obtainable::SIZE_ = Vector2(15, 15);

  Obtainable::Obtainable(World* world, entity_type type, Entity* content, const Vector2& position) :
    Entity(world, Entity::GROUP_OBTAINABLE, type),
    content_(content) {
    position_= position;
  }

  Obtainable::~Obtainable() {

  }

  Entity* Obtainable::content() {
    return content_;
  }

  const AABB Obtainable::collider() const {
    return AABB(Rect(position_, Obtainable::SIZE_));
  }
  
}