// shot_gun.h
#ifndef SHOT_GUN_H_
#define SHOT_GUN_H_

#include "weapon.h"

namespace nan2 {

  class ShotGun : public Weapon {

  private:

  public:

    ShotGun(World* world);

    void Update();
    bool Fire(unsigned char dir);

  };
  
}

#endif