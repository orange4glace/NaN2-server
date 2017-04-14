// weapon.cpp
#include "entity/weapon.h"

#include "entity/character.h"

namespace nan2 {

  Weapon::Weapon(Character* character, short id, float max_cool_down, float max_reload_time, const Vector2& firing_point) :
    Updatable(&(character->world())),
    id_(id),
    max_cool_down_(max_cool_down),
    max_reload_time_(max_reload_time),
    firing_point_(firing_point) {
  }

  void Weapon::Update() {
    DecreaseByDT(cooldown_);
    DecreaseByDT(reload_time_);
  }
  
}