// entity.h
#ifndef ENTITY_H
#define ENTITY_H

#include <nan2/math/vector2.h>

namespace nan2 {

  // Forward declaration
  class World;

  typedef unsigned char entity_type;
  typedef unsigned short entity_id;

  class Entity {

  private:

    static int next_internal_id_;

  protected:
    
    int internal_id_;
    entity_id id_;
    entity_type type_;

    World* world_;
    int layer_;

    Vector2 position_;

  public:

    Entity(World* world);
    
    World& world();
    int internal_id() const;
    entity_id id() const;
    entity_type type() const;
    const Vector2& position() const;
    virtual void OnCreate() {};
    virtual void OnDestroy() {};
    virtual void Destroy() = 0;

    void id(entity_id id);
    void position(const Vector2& pos);
    void position(float x, float y);

    int layer() const;
    void layer(int value);

  };
  
}

#endif