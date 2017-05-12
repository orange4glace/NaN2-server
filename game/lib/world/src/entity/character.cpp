// character.cpp
#include "entity/character.h"

// Forward declaration in header file
#include "world/world.h"
#include "entity/player.h"
#include "world/world_map.h"
#include "entity/machine_gun.h"

#include "math_helper.h"
#include "time.h"

#include "logger/logger.h"

#include <vector>
#include <algorithm>

namespace nan2 {

  const Vector2 Character::COLLIDER_SIZE_ = Vector2(10, 20);
  const Vector2& Character::COLLIDER_SIZE() {
    return Character::COLLIDER_SIZE_;
  }
  const float Character::SPEED_         = 115.0f;
  const int   Character::DASH_DURATION_ = 500;
  const float Character::DASH_DISTANCE_ = 100.0f;
  const int   Character::DASH_COOLDOWN_ = 1000;

  Character::Character(Player* player) :
    Updatable(&(player->world())),
    player_(player),
    weapon_(nullptr),
    last_input_acked_packet_(0),
    dash_cooldown_(0),
    dashing_(false) {
      update_order_ = 1;
      SetWeapon(new MachineGun(this));
      hp_ = max_hp_ = 10;
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

  void Character::Dash(const Vector2& angle) {
    dash_duration_ = Character::DASH_DURATION_;
    dash_cooldown_ = Character::DASH_COOLDOWN_;
    dashing_ = true;
  }

  void Character::Update() {
    k += 1000;
    if (k > 1000 * 60) k = 0;
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

      Move252(packet.move_dir(), fconsuming_time);

      if (is_fresh) {
        bool fire_result = Fire(packet.fire_dir());
        if (fire_result) {
          int bullet_packet_time = Time::current_time() - world_->last_snapshot_sent_time();
          BulletPacket bulletPacket(bullet_packet_time, 0, packet.fire_dir());
          snapshot_.AddBulletPacket(bulletPacket);
        }
      }
      snapshot_.SetWeaponMagazine(weapon_->magazine());
      snapshot_.SetWeaponAmmo(weapon_->ammo());
      snapshot_.SetWeaponCooldown(weapon_->cooldown());
      snapshot_.SetWeaponReloadTime(weapon_->reload_time());

      if (given_time <= 0) break;
      else {
        if (packet.time() <= 0) packets_.pop_front();
      }
    }

    DecreaseByDT(dash_cooldown_);
    DecreaseByDT(dash_duration_);
    if (dashing_) {
      float dash_t = std::max(Time::delta_time(), dash_duration_);
      float dd = (dash_t / Character::DASH_DURATION_);
      if (dash_duration_ == 0) {
        dashing_ = false;
      }
    }
    snapshot_.SetPosition(position_);
    snapshot_.SetDashingDuration(0);
    CharacterTickData tickData(Time::current_fixed_time(), position_, true);
    SaveTickData(tickData);
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
      AABB cur = collider();
      Vector2 dv = AABB::SimpleAABB(cur, aabb);
      position_ += dv;
    }
  }

  void Character::AddInput(PlayerInputPacket& plp) {
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

  const Vector2& Character::position() const {
    return position_;
  }

  int Character::hp() const {
    return hp_;
  }

  CharacterSnapshot& Character::snapshot() {
    return snapshot_;
  }

  void Character::position(float x, float y) {
    position_.set_x(x);
    position_.set_y(y);
  }

  void Character::position(const Vector2& v) {
    position_ = v;
  }

  void Character::hp(int hp) {
    hp_ = hp;
  }

  void Character::SetWeapon(Weapon* weapon) {
    world_->DestroyUpdatable(weapon_);
    if (weapon_ != nullptr) delete weapon_;
    weapon_ = weapon;
    world_->AddUpdatable(weapon);
  }

  const CharacterTickData Character::GetInterpolatedDataAt(int time) const {
    CharacterTickData q(time, Vector2(0, 0), true);
    auto it = std::lower_bound(history_.begin(), history_.end(), q);
    L_DEBUG << "Get interpolated " << time << " (lower bound = " << *it;
    if (it->time() == time) return *it;
    if (it == history_.begin()) return *it;
    L_DEBUG << "Interpolate " << time << " " << *std::prev(it) << " " << *it << " (last = " << *history_.rbegin() << ")";
    return CharacterTickData::Interpolate(*std::prev(it), *it, time);
  }
  
}