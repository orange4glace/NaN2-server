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

    int max_cooldown_;
    int cooldown_;
    int max_reload_time_;
    int reload_time_;
    int max_ammo_;
    int ammo_;
    int max_magazine_;
    int magazine_;
    bool is_reloading_;

    Vector2 bullet_point_;

    bool CanFire();
    void AfterFired();

    Vector2 GetBulletPoint(unsigned char dir, const Vector2& position) const;


  public:

    Weapon(Character* character, short id, int max_cooldown, int max_reload_time, int max_ammo, int max_magazine, const Vector2& bullet_point);

    void Update();
    // Fire weapon
    virtual bool Fire(unsigned char dir) = 0;
    int ammo();
    int magazine();
    int cooldown();
    int reload_time();


  };
  
}

#endif