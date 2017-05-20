// machine_gun.h
#ifndef MACHINE_GUN_H_
#define MACHINE_GUN_H_

#include "weapon.h"

namespace nan2 {

  class MachineGun : public Weapon {

  private:

  public:

    MachineGun(Character* character);

    void Update();
    bool Fire(unsigned char dir);

  };
  
}

#endif