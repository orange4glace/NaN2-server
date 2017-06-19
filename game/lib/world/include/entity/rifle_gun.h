// rifle_gun.h
#ifndef RIFLE_GUN_H_
#define RIFLE_GUN_H_

#include "weapon.h"

namespace nan2 {

  class RifleGun : public Weapon {

  private:

  public:

    RifleGun(World* world);

    void Update();
    bool Fire(unsigned char dir);

  };
  
}

#endif