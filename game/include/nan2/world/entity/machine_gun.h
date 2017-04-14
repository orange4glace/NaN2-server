// machine_gun.h
#ifndef MACHINE_GUN_H
#define MACHINE_GUN_H

#include "weapon.h"

namespace nan2 {

  class MachineGun : public Weapon {

  private:

  public:

    MachineGun(Character* character);

    void Update();
    void Fire(const Vector2& angle);

  };
  
}

#endif