// machine_gun.cpp
#include "entity/machine_gun.h"

#include "entity/machine_gun_bullet.h"
#include "world/world.h"
#include "entity/character.h"

namespace nan2 {

  MachineGun::MachineGun(Character* character) :
    Weapon(character, 1, 0.2f, 1.0f, Vector2(15, 0))
    {
  }

  void MachineGun::Update() {
    Weapon::Update();
  }

  void MachineGun::Fire(const Vector2& angle) {
    Bullet* bullet = new MachineGunBullet(world_, character_->position() + firing_point_, angle, 0);
    world_->AddUpdatable(bullet);
  }
  
}