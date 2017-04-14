// updatable.cpp
#include "entity/updatable.h"

#include "world/world.h"

namespace nan2 {

  Updatable::Updatable(World* world) :
    Entity(world),
    is_on_stage_(false) {
  }
  
  const int Updatable::update_order() const {
    return update_order_;
  }
  Updatable& Updatable::set_is_on_stage(bool v) {
    is_on_stage_ = v;
    return *this;
  }
  const bool Updatable::is_on_stage() const {
    return is_on_stage_;
  }

  void Updatable::Destroy() {
    world_->DestroyUpdatable(this);
  }

}