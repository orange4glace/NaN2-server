// character.h
#ifndef CHARACTER_H
#define CHARACTER_H

#include "updatable.h"
#include "collider_interface.h"
#include "weapon.h"
#include "character_tick_data.h"
#include "../network/tick_packet.h"

#include <nan2/math/vector2.h>

#include <string>
#include <deque>

namespace nan2 {

  // Forward declaration
  class Player;

  class Character : public Updatable, public ColliderInterface {

  private:

    const static Vector2 COLLIDER_SIZE_;
    const static float DASH_DURATION_;
    const static float DASH_DISTANCE_;
    const static float DASH_COOLDOWN_;

    Player* player_;

    Vector2 position_;
    bool controllable_;
    int max_hp_;
    int hp_;
    float dash_duration_;
    float dash_distance_;
    float dash_cooldown_;
    bool dashing_;

    Weapon* weapon_;

    std::deque<CharacterTickData> history_;
    std::deque<TickPacket> packets_;

    void SaveTickData();

  public:

    static const Vector2& COLLIDER_SIZE();

    Character(Player* player);

    // ColliderInterface
    virtual const AABB collider() const;

    // Fire weapon
    void Fire(const Vector2& angle) const;
    void Dash(const Vector2& angle);
    void Update();
    void Move256(unsigned char dir, float time);
    void Move(float dx, float dy);
    void MoveTo(float x, float y);

    const Player& player() const;
    const Vector2& position() const;
    void set_position(float x, float y);
    void set_position(const Vector2& v);
    void set_weapon(Weapon* weapon);

    const CharacterTickData GetInterpolatedDataAt(int time) const;

  };
  
}

#endif