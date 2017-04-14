// character_tick_data.cpp
#include "entity/character_tick_data.h"

#include "entity/character.h"

namespace nan2 {
  
  CharacterTickData::CharacterTickData(int time, const Vector2& position, bool is_alive) :
    time_(time),
    position_(position),
    collider_(Rect(position.x(), position.y(), Character::COLLIDER_SIZE().x(), Character::COLLIDER_SIZE().y())),
    is_alive_(is_alive) {

  }

  const int CharacterTickData::time() const {
    return time_;
  }
  const Vector2& CharacterTickData::position() const {
    return position_;
  }
  const AABB& CharacterTickData::collider() const {
    return collider_;
  }
  const bool CharacterTickData::is_alive() const {
    return is_alive_;
  }

  bool CharacterTickData::operator<(const CharacterTickData& o) const {
    return time_ < o.time_;
  }

  CharacterTickData CharacterTickData::Interpolate(CharacterTickData a, CharacterTickData b, int t) {
    if (a.time() > b.time()) {
      CharacterTickData c = a;
      a = b;
      b = c;
    }
    if (a.time() >= t) return a;
    if (b.time() <= t) return b;
    float k = (t - a.time()) / (b.time() - a.time());
    return CharacterTickData(t, (b.position() - a.position()) * k + a.position(), a.is_alive());
  }

}