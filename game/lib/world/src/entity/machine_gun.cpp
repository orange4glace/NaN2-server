// machine_gun.cpp
#include "entity/machine_gun.h"

#include "entity/machine_gun_bullet.h"
#include "world/world.h"
#include "math_helper.h"

namespace nan2 {

  MachineGun::MachineGun(World* world) :
    Weapon(world, Entity::TYPE_MACHINE_GUN, 1,
      80, 1000, 30, 140,
      Vector2(29, 0))
    {
  }

  void MachineGun::Update() {
    Weapon::Update();
  }

  bool MachineGun::Fire(unsigned char dir) {
    if (!CanFire()) return false;
    Vector2 bullet_position = GetBulletPoint(dir,character_->position());
    Vector2 dir_vec = MathHelper::instance().normal_dir_252(dir);
    Bullet* bullet = new MachineGunBullet(world_, bullet_position, dir_vec, character_->player().id(), 0);
    world_->AddUpdatable(bullet);
    AfterFired();
    return true;
  }
  
}