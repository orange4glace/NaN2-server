// shot_gun.cpp
#include "entity/shot_gun.h"

#include "entity/machine_gun_bullet.h"
#include "world/world.h"
#include "math_helper.h"

namespace nan2 {

  ShotGun::ShotGun(World* world) :
    Weapon(world, Entity::TYPE_SHOT_GUN,
      400, 1000, 8, 10000,
      Vector2(23, 3))
    {
  }

  void ShotGun::Update() {
    Weapon::Update();
  }

  bool ShotGun::Fire(unsigned char dir) {
    if (!CanFire()) return false;
    int freqA = ammo_;
    for (int i = 0; i < 5; i ++) {
      freqA *= ammo_ *131071;
      freqA %= 524287;
      int dir_ = dir;
      int freq = (freqA % 5) - 2;
      dir_ += freq;
      dir_ = (dir_ + 252) % 252;
      dir = (unsigned char)dir_;

      Vector2 bullet_position = GetBulletPoint(dir,character_->position());
      Vector2 dir_vec = MathHelper::instance().normal_dir_252(dir);
      Bullet* bullet = new MachineGunBullet(world_, bullet_position, dir_vec, Vector2(5, 5), 450.0f, 5, character_->player().id(), 0);
      world_->AddUpdatable(bullet);
    }
    AfterFired();
    return true;
  }
  
}