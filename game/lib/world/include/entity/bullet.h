// bullet.h
#ifndef BULLET_H
#define BULLET_H

#include "updatable.h"
#include "collider_interface.h"

namespace nan2 {

  class Bullet : public Updatable, public ColliderInterface {

  protected:

    int collision_mask_;
    int damage_;
    int player_id_;

    int created_time_;
    int alive_time_;
    int interpolation_time_;

    Bullet(World* world_, int player_id_, int collision_mask_);

  public :

    virtual const AABB collider() const = 0;

    int player_id() const;

    void OnCreate();
    void Update();

  };
  
}

#endif