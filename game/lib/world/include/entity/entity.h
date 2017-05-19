// entity.h
#ifndef ENTITY_H
#define ENTITY_H

#include <nan2/math/vector2.h>

namespace nan2 {

  // Forward declaration
  class World;

  class Entity {

  private:
    
    int internal_id_;

  protected:

    static int next_internal_id_;

    World* world_;
    int layer_;

  public:

    Entity(World* world);
    
    World& world();
    int internal_id() const;
    virtual void OnCreate() {};
    virtual void OnDestroy() {};
    virtual void Destroy() = 0;

    int layer() const;
    void layer(int value);

  };
  
}

#endif