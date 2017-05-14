// character_snapshot.cpp
#include "network/character_snapshot.h"

namespace nan2 {

  CharacterSnapshot::CharacterSnapshot() {}

  void CharacterSnapshot::AddBulletPacket(BulletPacket& bullet_packet) {
    bullet_packets_.push_back(bullet_packet);
  }

  void CharacterSnapshot::SetPosition(const Vector2& position) {
    position_ = position;
  }

  void CharacterSnapshot::SetDashingDuration(int dashing_duration) {
    dashing_duration_ = dashing_duration;
  }

  void CharacterSnapshot::SetWeaponMagazine(int weapon_magazine) {
    weapon_magazine_ = weapon_magazine;
  }

  void CharacterSnapshot::SetWeaponAmmo(int weapon_ammo) {
    weapon_ammo_ = weapon_ammo;
  }

  void CharacterSnapshot::SetWeaponCooldown(int weapon_cooldown) {
    weapon_cooldown_ = weapon_cooldown;
  }

  void CharacterSnapshot::SetWeaponReloadTime(int weapon_reload_time) {
    weapon_reload_time_ = weapon_reload_time;
  }

  void CharacterSnapshot::Update(int dt) {
    snapshot_time_ += dt;
  }

  const int CharacterSnapshot::snapshot_time() {
    return snapshot_time_;
  }

  const std::vector<BulletPacket>& CharacterSnapshot::bullet_packets() {
    return bullet_packets_;
  }

  const Vector2& CharacterSnapshot::position() {
    return position_;
  }

  const int CharacterSnapshot::dashing_duration() {
    return dashing_duration_;
  }

  const int CharacterSnapshot::weapon_magazine() {
    return weapon_magazine_;
  }

  const int CharacterSnapshot::weapon_ammo() {
    return weapon_ammo_;
  }

  const int CharacterSnapshot::weapon_cooldown() {
    return weapon_cooldown_;
  }

  const int CharacterSnapshot::weapon_reload_time() {
    return weapon_reload_time_;
  }

  void CharacterSnapshot::Clear() {
    bullet_packets_.clear();
    snapshot_time_ = 0;
    dashing_duration_  = 0;
    position_ = Vector2::ZERO;
  }

  std::ostream& operator<<(std::ostream& os, const CharacterSnapshot& snapshot) {
      os << "[Snapshot] " << snapshot.snapshot_time_ << " " << snapshot.position_;
      return os;
  }
}