// entity.cpp
#include "entity/entity.h"

#include "world/world.h"

#include <cassert>

namespace nan2 {

  int Entity::next_internal_id_ = 0;

  Entity::Entity(World* world, entity_group group, entity_type type) :
    world_(world),
    group_(group),
    type_(type) {
      assert(world != nullptr);
      internal_id_ = Updatable::next_internal_id_++;
  }

  int Entity::internal_id() const {
    return internal_id_;
  }

  entity_id Entity::id() const {
    return id_;
  }

  entity_group Entity::group() const {
    return group_;
  }

  entity_type Entity::type() const {
    return type_;
  }

  const Vector2& Entity::position() const {
    return position_;
  }

  World& Entity::world() {
    return *world_;
  }

  int Entity::layer() const {
    return layer_;
  }

  void Entity::id(entity_id id) {
    id_ = id;
  }

  void Entity::position(const Vector2& v) {
    position_ = v;
  }

  void Entity::position(float x, float y) {
    position_.set_x(x);
    position_.set_y(y);
  }


  void Entity::layer(int value) {
    layer_ = value;
  }
  
}