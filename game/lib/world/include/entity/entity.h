// entity.h
#ifndef ENTITY_H
#define ENTITY_H

#include <nan2/math/vector2.h>

namespace nan2 {

  // Forward declaration
  class World;

  class Entity {

  public:
    Entity(World* world);

  protected:
    
    World* world_;
    short layer;

  public:
    
    World& world();
    virtual void OnCreate() {};
    virtual void OnDestroy() {};
    virtual void Destroy() = 0;

  };
  
}

#endif