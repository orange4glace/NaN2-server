// weapon.h
#ifndef WEAPON_H
#define WEAPON_H

#include "updatable.h"
#include <nan2/math/vector2.h>

namespace nan2 {

  // Forward Declaration
  class Character;

  class Weapon : public Updatable {

  protected:

    // Weapon id
    short id_;
    Character* character_;

    float max_cool_down_;
    float cooldown_;
    float max_reload_time_;
    float reload_time_;
    bool is_reloading_;

    Vector2 firing_point_;



  public:

    Weapon(Character* character, short id, float max_cool_down, float max_reload_time, const Vector2& firing_point);

    void Update();
    // Fire weapon
    virtual bool Fire(const Vector2& angle) = 0;


  };
  
}

#endif