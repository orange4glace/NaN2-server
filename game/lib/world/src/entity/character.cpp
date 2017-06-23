// character.cpp
#include "entity/character.h"

// Forward declaration in header file
#include "world/world.h"
#include "entity/player.h"
#include "world/world_map.h"
#include "entity/machine_gun.h"
#include "entity/rifle_gun.h"
#include "entity/shot_gun.h"

#include "math_helper.h"
#include "time.h"

#include "logger/logger.h"

#include <vector>
#include <utility>
#include <algorithm>

namespace nan2 {

  const Vector2 Character::COLLIDER_SIZE_ = Vector2(8, 18);
  const Vector2& Character::COLLIDER_SIZE() {
    return Character::COLLIDER_SIZE_;
  }
  const float Character::SPEED_         = 95.0f;
  const int   Character::DASH_DURATION_ = 300;
  const float Character::DASH_DISTANCE_ = 70.0f;
  const int   Character::DASH_COOLDOWN_ = 1000;
  const int   Character::RESPAWN_TIME = 5000;

  Character::Character(Player* player) :
    Updatable(&(player->world()), Entity::GROUP_CHARACTER, Entity::TYPE_CHARACTER),
    player_(player),
    weapon_(nullptr),
    last_input_acked_packet_(0),
    dash_cooldown_(0),
    dash_dir_(0),
    dash_time_(0),
    dashing_(false),
    respawn_counter_(0) {
      position_ = Vector2(1000, 500);
      update_order_ = 1;
      SetWeapon(new ShotGun(world_));
      hp_ = max_hp_ = 50;
  }

  const AABB Character::collider() const {
    return AABB(Rect(position_.x(), position_.y(), Character::COLLIDER_SIZE_.x(), Character::COLLIDER_SIZE_.y()));
  }

  void Character::SaveTickData(const CharacterTickData& tick_data) {
    history_.push_back(tick_data);
  }

  bool Character::Fire(unsigned char dir) const {
    if (weapon_ == nullptr) return false;
    if (dir >= 252) return false;
    return weapon_->Fire(dir);
  }

  void Character::Update() {
    k += 1000;
    if (k > 1000 * 60) k = 0;

    AliveCheck();
    RespawnCheck();
  }

  void Character::FixedUpdate() {
    int dt = Time::fixed_delta_time();
    snapshot_.Update(dt);
    int given_time = dt;
    while (!packets_.empty()) {
      PlayerInputPacket& packet = packets_.front();
      bool is_fresh = !packet.is_consuming();
      int consuming_time = packet.Consume(given_time);
      given_time -= consuming_time;
      float fconsuming_time = consuming_time / 1000.0f;

      last_input_acked_packet_ = packet.sequence();
      last_input_remaining_time_ = packet.time();

      // Alive check
      if (is_alive()) {

        if (packet.dash_dir() < 252) {
          // Player dash command
          Dash(packet.dash_dir());
        }
        if (!dashing_)
          Move252(packet.move_dir(), fconsuming_time);
        UpdateDashState(consuming_time);

        if (is_fresh) {
          bool fire_result = Fire(packet.fire_dir());
          if (fire_result) {
            int bullet_packet_time = Time::current_time() - world_->last_snapshot_sent_time();
            BulletPacket bulletPacket(bullet_packet_time, 0, packet.fire_dir());
            snapshot_.AddBulletPacket(bulletPacket);
          }
        }
      }

      if (given_time <= 0) break;
      else {
        if (packet.time() <= 0) packets_.pop_front();
      }
    }
    TryObtainItem();

    snapshot_.SetWeaponMagazine(weapon_->magazine());
    snapshot_.SetWeaponAmmo(weapon_->ammo());
    snapshot_.SetWeaponCooldown(weapon_->cooldown());
    snapshot_.SetWeaponReloadTime(weapon_->reload_time());
    snapshot_.SetPosition(position_);
    snapshot_.SetDashTime(dash_time_);
    snapshot_.SetDashCooldown(dash_cooldown_);
    snapshot_.SetDashDir(dash_dir_);
    CharacterTickData tickData(Time::current_fixed_time(), position_, true);
    SaveTickData(tickData);

    while (history_.size()) {
      CharacterTickData& td = history_.front();
      if (td.time() + 2000 < Time::current_fixed_time()) history_.pop_front();
      else break;
    }
  }

  void Character::Move252(unsigned char dir, float time) {
    if (dir >= 252) return;
    Vector2 dv = MathHelper::instance().normal_dir_252(dir) * time * Character::SPEED_;
    Move(dv.x(), dv.y());
  }

  void Character::Move(float dx, float dy) {
    MoveTo(position_.x() + dx, position_.y() + dy);
  }

  void Character::MoveTo(float x, float y) {
    const std::vector<AABB>& static_map_colliders = world_->world_map()->GetStaticAABBTileColliders();
    position_ = Vector2(x, y);
    for (AABB aabb : static_map_colliders) {
      bool collided;
      AABB cur = collider();
      Vector2 dv = AABB::SimpleAABB(cur, aabb, collided);
      position_ += dv;
    }
  }

  void Character::Dash(unsigned char dir) {
    if (dir >= 252) return;
    if (dash_cooldown_) return;
    dashing_ = true;
    dash_time_ = 1;
    dash_dir_ = dir;
    dash_cooldown_ = Character::DASH_COOLDOWN_;
  }

  void Character::AliveCheck() {
    if (hp_ == 0 && respawn_counter_ == 0) 
      respawn_counter_ = 1;
  }

  void Character::RespawnCheck() {
    if (respawn_counter_ > 0)
      respawn_counter_ += Time::delta_time();
    if (respawn_counter_ >= Character::RESPAWN_TIME) {
      // Let's respawn
      hp_ = max_hp_;
      respawn_counter_ = 0;
      L_DEBUG << "Character " << player_->id() << " Respawned.";
    }
  }

  void Character::UpdateDashState(int dt) {
    DecreaseByDT(dash_cooldown_);
    if (!dashing_) return;
    dash_time_ += dt;
    int dash_update_time = dt;
    if (dash_time_ >= Character::DASH_DURATION_)
      dash_update_time = dt - (dash_time_ - Character::DASH_DURATION_);
    float dd = Character::DASH_DISTANCE_ * ((float)dash_update_time / (Character::DASH_DURATION_));
    Vector2 vdd = MathHelper::instance().normal_dir_252(dash_dir_) * dd;
    Move(vdd.x(), vdd.y());
    if (dash_time_ > Character::DASH_DURATION_) {
      dash_time_ = 0;
      dashing_ = false;
    }
  }

  bool Character::AddEntityToInventory(Entity* entity) {
    std::pair<std::map<int, Entity*>::iterator, bool> ret =
      inventory_.insert(std::pair<int, Entity*>(entity->id(), entity));
    return ret.second;
  }

  Entity* Character::GetEntityFromInventory(int id) {
    return inventory_[id];
  }

  Entity* Character::TryObtainItem() {
    float rf = INFINITY;
    AABB cur = collider();
    Obtainable* rObtainable = nullptr;
    world_->IterateEntityGroup(Entity::GROUP_OBTAINABLE, [&](Entity* entity)->bool {
      bool collided;
      Obtainable* obtainable = (Obtainable*)entity;
      const AABB& collider = obtainable->collider();
      Vector2 dv = AABB::SimpleAABB(cur, collider, collided);
      float cf = dv.GetMagnitude();
      if (!collided) return true;
      if (cf < rf) {
        rObtainable = obtainable;
        rf = cf;
      }
      return true;
    });
    if (rObtainable != nullptr) {
      L_DEBUG << "Player " << player_->id() << " obtained " << rObtainable->id();
      AddEntityToInventory(rObtainable);
      Entity* content = rObtainable->content();
      if (content->group() == Entity::GROUP_WEAPON) {
        Updatable* updatable = (Updatable*)content;
        world_->AddUpdatable(updatable);
        Weapon* weapon = (Weapon*)updatable;
        L_DEBUG << "obtain "<< weapon->id() << " " << weapon->type();

        if (weapon->type() != weapon_->type())
          SetWeapon(weapon);
      }
      // Netcode
      net_entities_obtained_.push(content);
      world_->AddEntityDestroiedPacket(rObtainable);

      rObtainable->Destroy();
    }
    return nullptr;
  }

  void Character::AddInput(const PlayerInputPacket& plp) {
    packets_.push_back(plp);
  }

  void Character::AddHP(int hp) {
    hp_ += hp;
    if (hp_ < 0) hp_ = 0;
    if (hp_ > max_hp_) hp_ = max_hp_;
  }

  const Player& Character::player() const {
    return *player_;
  }

  int Character::hp() const {
    return hp_;
  }

  bool Character::is_dashing() const {
    return dash_time_ > 0;
  }

  CharacterSnapshot& Character::snapshot() {
    return snapshot_;
  }

  void Character::hp(int hp) {
    hp_ = hp;
  }

  bool Character::is_alive() const {
    return hp_ > 0;
  }

  const Weapon* Character::weapon() const {
    return weapon_;
  }

  void Character::SetWeapon(Weapon* weapon) {
    world_->DestroyUpdatable(weapon_);
    weapon_ = weapon;
    weapon->SetCharacter(this);
    world_->AddUpdatable(weapon);
  }

  const CharacterTickData Character::GetInterpolatedDataAt(int time) const {
    CharacterTickData q(time, Vector2(0, 0), true);
    auto it = std::lower_bound(history_.begin(), history_.end(), q);
    if (it->time() == time) return *it;
    if (it == history_.begin()) return *it;
    return CharacterTickData::Interpolate(*std::prev(it), *it, time);
  }
  
}
