// entity.cpp
#include "entity/entity.h"

#include "world/world.h"

#include <cassert>

namespace nan2 {

  Entity::Entity(World* world) :
    world_(world) {
      assert(world != nullptr);
  }

  World& Entity::world() {
    return *world_;
  }

  int Entity::layer() const {
    return layer_;
  }

  void Entity::layer(int value) {
    layer_ = value;
  }
  
}