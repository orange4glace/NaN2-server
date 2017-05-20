// breakable.cpp
#include "entity/breakable.h"

namespace nan2 {

  Breakable::Breakable(World* world, const Vector2& position) :
    Entity(world) {
    position_= position;
  }

  void Breakable::Destroy() {

  }

}