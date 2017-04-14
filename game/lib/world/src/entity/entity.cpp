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
  
}