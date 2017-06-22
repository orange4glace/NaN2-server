// machine_gun.cpp
#include "entity/machine_gun.h"

#include "entity/machine_gun_bullet.h"
#include "world/world.h"
#include "math_helper.h"

namespace nan2 {

  MachineGun::MachineGun(World* world) :
    Weapon(world, Entity::TYPE_MACHINE_GUN,
      80, 1000, 30, 10000,
      Vector2(29, 0))
    {
  }

  void MachineGun::Update() {
    Weapon::Update();
  }

  bool MachineGun::Fire(unsigned char dir) {
    if (!CanFire()) return false;
    int _dir = dir;
    int freq = ((ammo_ * ammo_) % 5) - 4;
    _dir = (_dir + freq + 252) % 252;
    dir = (unsigned char)_dir;
    Vector2 bullet_position = GetBulletPoint(dir,character_->position());
    Vector2 dir_vec = MathHelper::instance().normal_dir_252(dir);
    Bullet* bullet = new MachineGunBullet(world_, bullet_position, dir_vec, Vector2(3, 3), 250.0f, 2, character_->player().id(), 0);
    world_->AddUpdatable(bullet);
    AfterFired();
    return true;
  }
  
}