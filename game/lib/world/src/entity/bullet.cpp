// bullet.cpp
#include "entity/bullet.h"

#include "entity/character.h"

namespace nan2 {

  Bullet::Bullet(World* world, const int collision_mask) :
    Updatable(world),
    collision_mask_(collision_mask) {
  }
  
}