// machine_gun_bullet.h
#ifndef MACHINE_GUN_BULLET_H
#define MACHINE_GUN_BULLET_H

#include "bullet.h"
#include <nan2/math/vector2.h>

namespace nan2 {

  class MachineGunBullet : public Bullet {

  private:

    Vector2 angle_;
    float SPEED_;

  public:

    MachineGunBullet(World* world, const Vector2& position, const Vector2& angle, int player_id, int collision_mask);

    virtual const AABB collider() const;

    void Update();
    void FixedUpdate();

  };
  
}

#endif