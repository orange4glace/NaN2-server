// entity.cpp
#include "entity/entity.h"

#include "world/world.h"

#include <cassert>

namespace nan2 {

  int Entity::next_internal_id_ = 0;

  Entity::Entity(World* world) :
    world_(world) {
      assert(world != nullptr);
      internal_id_ = Updatable::next_internal_id_++;
  }

  int Entity::internal_id() const {
    return internal_id_;
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