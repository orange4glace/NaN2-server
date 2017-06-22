// character.h
#ifndef CHARACTER_H
#define CHARACTER_H

#include "updatable.h"
#include "collider_interface.h"
#include "weapon.h"
#include "character_tick_data.h"
#include "../network/player_input_packet.h"
#include "../network/character_snapshot.h"
#include "../network/bullet_packet.h"

#include <nan2/math/vector2.h>

#include <string>
#include <map>
#include <queue>
#include <deque>

namespace nan2 {

  // Forward declaration
  class Player;

  class Character : public Updatable, public ColliderInterface {

  private:

    const static Vector2 COLLIDER_SIZE_;
    const static float SPEED_;
    const static int DASH_DURATION_;
    const static float DASH_DISTANCE_;
    const static int DASH_COOLDOWN_;
    const static int RESPAWN_TIME;

    Player* player_;

    bool controllable_;
    int max_hp_;
    int hp_;
    int dash_duration_;
    float dash_distance_;
    int dash_cooldown_;
    int dash_time_;
    unsigned char dash_dir_;
    bool dashing_;
    int respawn_counter_;

    Weapon* weapon_;
    std::map<int, Entity*> inventory_;

    std::deque<CharacterTickData> history_;
    std::deque<PlayerInputPacket> packets_;
    CharacterSnapshot snapshot_;

    void SaveTickData(const CharacterTickData& tick_data);
    void UpdateDashState(int dt);
    void Dash(unsigned char dir);
    void AliveCheck();
    void RespawnCheck();

    bool AddEntityToInventory(Entity* entity);
    Entity* GetEntityFromInventory(int id);

    Entity* TryObtainItem();

    float k;

  public:

    // tmp
    unsigned int last_input_acked_packet_;
    unsigned int last_input_remaining_time_;
    std::queue<Entity*> net_entities_obtained_;
    std::queue<Entity*> net_entities_destroied_;

    static const Vector2& COLLIDER_SIZE();

    Character(Player* player);

    // ColliderInterface
    virtual const AABB collider() const;

    // Fire weapon
    bool Fire(unsigned char dir) const;
    void Update();
    void FixedUpdate();
    void Move252(unsigned char dir, float time);
    void Move(float dx, float dy);
    void MoveTo(float x, float y);
    void AddHP(int hp);

    // Network function
    void AddInput(const PlayerInputPacket& plp);

    const Player& player() const;
    int hp() const;
    bool is_dashing() const;
    CharacterSnapshot& snapshot();
    void hp(int hp);
    const Weapon* weapon() const;
    void SetWeapon(Weapon* weapon);
    bool is_alive() const;

    const CharacterTickData GetInterpolatedDataAt(int time) const;

  };
  
}

#endif