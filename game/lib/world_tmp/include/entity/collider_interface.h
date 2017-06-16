// collider.h
#ifndef COLLIDER_INTERFACE_H
#define COLLIDER_INTERFACE_H

#include <nan2/math/aabb.h>

namespace nan2 {

  class ColliderInterface {

  public:
    
    virtual const AABB collider() const = 0;

  };
  
}

#endif