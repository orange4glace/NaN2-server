// rifle_gun.cpp
#include "entity/rifle_gun.h"

#include "entity/machine_gun_bullet.h"
#include "world/world.h"
#include "math_helper.h"

namespace nan2 {

  RifleGun::RifleGun(World* world) :
    Weapon(world, Entity::TYPE_RIFLE_GUN,
      1000, 1000, 5, 10000,
      Vector2(23, 3))
    {
  }

  void RifleGun::Update() {
    Weapon::Update();
  }

  bool RifleGun::Fire(unsigned char dir) {
    if (!CanFire()) return false;
    Vector2 bullet_position = GetBulletPoint(dir,character_->position());
    Vector2 dir_vec = MathHelper::instance().normal_dir_252(dir);
    Bullet* bullet = new MachineGunBullet(world_, bullet_position, dir_vec, Vector2(5, 5), 2000.0f, 15, character_->player().id(), 0);
    world_->AddUpdatable(bullet);
    AfterFired();
    return true;
  }
  
}