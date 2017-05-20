// entity.h
#ifndef ENTITY_H_
#define ENTITY_H_

#include <nan2/math/vector2.h>

namespace nan2 {

  // Forward declaration
  class World;

  typedef unsigned short entity_id;
  typedef unsigned char entity_group;
  typedef unsigned char entity_type;

  class Entity {

  private:

    static int next_internal_id_;

  protected:
    
    int internal_id_;
    entity_id id_;
    entity_group group_;
    entity_type type_;

    World* world_;
    int layer_;

    Vector2 position_;

  public:

    static const entity_group GROUP_CHARACTER = 1;
    static const entity_group GROUP_WEAPON = 2;
    static const entity_group GROUP_BULLET = 3;
    static const entity_group GROUP_BREAKABLE = 4;
    static const entity_group GROUP_DROPPED_ITEM = 5;

    static const entity_type TYPE_BREAKABLE = 1;
    static const entity_type TYPE_DROPPED_ITEM = 2;

    Entity(World* world);
    
    World& world();
    int internal_id() const;
    entity_id id() const;
    entity_group group() const;
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