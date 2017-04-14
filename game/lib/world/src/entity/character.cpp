// character.cpp
#include "entity/character.h"

// Forward declaration in header file
#include "world/world.h"
#include "entity/player.h"
#include "world/world_map.h"

#include "math_helper.h"
#include "time.h"

#include <vector>
#include <algorithm>

namespace nan2 {

  const Vector2 Character::COLLIDER_SIZE_ = Vector2(10, 20);
  const Vector2& Character::COLLIDER_SIZE() {
    return Character::COLLIDER_SIZE_;
  }

  Character::Character(Player* player) :
    Updatable(&(player->world())),
    player_(player),
    dash_cooldown_(0),
    dashing_(false) {
  }

  const AABB Character::collider() const {
    return AABB(Rect(position_.x(), position_.y(), Character::COLLIDER_SIZE_.x(), Character::COLLIDER_SIZE_.y()));
  }

  void Character::SaveTickData() {
    int time = Time::current_time();
    CharacterTickData tickData(time, position_, true);
    history_.push_back(tickData);
  }

  void Character::Fire(const Vector2& angle) const {
    if (weapon_ == nullptr) return;
    weapon_->Fire(angle);
  }

  void Character::Dash(const Vector2& angle) {
    dash_duration_ = Character::DASH_DURATION_;
    dash_cooldown_ = Character::DASH_COOLDOWN_;
    dashing_ = true;
  }

  void Character::Update() {
    float given_time = Time::delta_time();
    while (packets_.empty()) {
      TickPacket packet = packets_.front();
      given_time = packet.Consume(given_time);
      if (given_time == 0) break;
      else packets_.pop_front();
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
    SaveTickData();
  }

  void Character::Move256(unsigned char dir, float time) {
    Vector2 dv = MathHelper::instance().normal_dir_256(dir) * time;
    MoveTo(position_.x() + dv.x(), position_.y() + dv.y());
  }

  void Character::Move(float dx, float dy) {
    MoveTo(position_.x() + dx, position_.y() + dy);
  }

  void Character::MoveTo(float x, float y) {
    const std::vector<AABB>& static_map_colliders = world_->world_map().GetStaticAABBTileColliders();
    for (AABB aabb : static_map_colliders) {
      AABB cur = collider();
      Vector2 dv = AABB::SimpleAABB(cur, aabb);
      position_ += dv;
    }
  }

  const Player& Character::player() const {
    return *player_;
  }

  const Vector2& Character::position() const {
    return position_;
  }

  void Character::set_position(float x, float y) {
    position_.set_x(x);
    position_.set_y(y);
  }

  void Character::set_position(const Vector2& v) {
    position_ = v;
  }

  void Character::set_weapon(Weapon* weapon) {
    if (weapon_ != nullptr) delete weapon_;
    weapon_ = weapon;
  }

  const CharacterTickData Character::GetInterpolatedDataAt(int time) const {
    CharacterTickData q(time, Vector2(0, 0), true);
    auto it = std::lower_bound(history_.begin(), history_.end(), q);
    if (it->time() == time) return *it;
    if (it == history_.begin()) return *it;
    return CharacterTickData::Interpolate(*std::prev(it), *it, time);
  }
  
}