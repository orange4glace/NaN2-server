// character_tick_data.h
#ifndef CHARACTER_TICK_DATA_H_
#define CHARACTER_TICK_DATA_H_

#include <nan2/math/aabb.h>
#include <nan2/math/vector2.h>

namespace nan2 {

  class CharacterTickData {

  private:
    int time_;
    Vector2 position_;
    AABB collider_;
    unsigned char fire_dir_;
    bool is_alive_;

  public:

    CharacterTickData(int time, const Vector2& position, bool is_alive);

    const int time() const;
    const Vector2& position() const;
    const AABB& collider() const;
    const bool is_alive() const;

    bool operator<(const CharacterTickData& o) const;

    static CharacterTickData Interpolate(CharacterTickData a, CharacterTickData b, int t);

  };
  
}

#endif