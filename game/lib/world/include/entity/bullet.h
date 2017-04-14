// bullet.h
#ifndef BULLET_H
#define BULLET_H

#include "updatable.h"
#include "collider_interface.h"

namespace nan2 {

  class Bullet : public Updatable, public ColliderInterface {

  protected:

    int collision_mask_;

    Bullet(World* world_, const int collision_mask_);

  public :

    virtual const AABB collider() const = 0;

  };
  
}

#endif