// machine_gun_bullet.h
#ifndef MACHINE_GUN_BULLET_H
#define MACHINE_GUN_BULLET_H

#include "bullet.h"
#include <nan2/math/vector2.h>

namespace nan2 {

  class MachineGunBullet : public Bullet {

  private:

    Vector2 position_;
    Vector2 angle_;
    float SPEED_;
    float interpolation_time_;

  public:

    MachineGunBullet(World* world, const Vector2& position, const Vector2& angle, const int collision_mask);

    virtual const AABB collider() const;

    void Update();

  };
  
}

#endif