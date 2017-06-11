// bullet.cpp
#include "entity/bullet.h"

#include "entity/character.h"

namespace nan2 {

  Bullet::Bullet(World* world, int player_id, int collision_mask) :
    Updatable(world, Entity::GROUP_BULLET, Entity::TYPE_BULLET),
    player_id_(player_id),
    collision_mask_(collision_mask) {
      update_order_ = 2;
      alive_time_ = 5000;
      interpolation_time_ = 0;
  }

  int Bullet::player_id() const {
    return player_id_;
  }

  void Bullet::OnCreate() {
    created_time_ = Time::current_time();
  }

  void Bullet::Update() {
    if (Time::current_time() - created_time_ > alive_time_) Destroy();
  }
  
}