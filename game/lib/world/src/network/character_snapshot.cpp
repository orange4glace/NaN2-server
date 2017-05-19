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

  void CharacterSnapshot::SetDashTime(int dash_time) {
    dash_time_ = dash_time;
  }

  void CharacterSnapshot::SetDashCooldown(int dash_cooldown) {
    dash_cooldown_ = dash_cooldown;
  }

  void CharacterSnapshot::SetDashDir(unsigned char dash_dir) {
    dash_dir_ = dash_dir;
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

  int CharacterSnapshot::snapshot_time() const{
    return snapshot_time_;
  }

  const std::vector<BulletPacket>& CharacterSnapshot::bullet_packets() const {
    return bullet_packets_;
  }

  const Vector2& CharacterSnapshot::position() const {
    return position_;
  }

  int CharacterSnapshot::dash_time() const {
    return dash_time_;
  }

  int CharacterSnapshot::dash_cooldown() const {
    return dash_cooldown_;
  }

  unsigned char CharacterSnapshot::dash_dir() const {
    return dash_dir_;
  }

  int CharacterSnapshot::weapon_magazine() const {
    return weapon_magazine_;
  }

  int CharacterSnapshot::weapon_ammo() const {
    return weapon_ammo_;
  }

  int CharacterSnapshot::weapon_cooldown() const {
    return weapon_cooldown_;
  }

  int CharacterSnapshot::weapon_reload_time() const {
    return weapon_reload_time_;
  }

  void CharacterSnapshot::Clear() {
    bullet_packets_.clear();
    snapshot_time_ = 0;
    dash_time_  = 0;
    dash_dir_ = 255;
    position_ = Vector2::ZERO;
  }

  std::ostream& operator<<(std::ostream& os, const CharacterSnapshot& snapshot) {
      os << "[Snapshot] " << snapshot.snapshot_time_ << " " << snapshot.position_;
      return os;
  }
}