// character_snapshot.cpp
#include "network/character_snapshot.h"

namespace nan2 {

  CharacterSnapshot::CharacterSnapshot() {}

  void CharacterSnapshot::AddBulletPacket(BulletPacket& bullet_packet) {
    bullet_packet.time(snapshot_time_);
    bullet_packets_.push_back(bullet_packet);
  }

  void CharacterSnapshot::SetPosition(const Vector2& position) {
    position_ = position;
  }

  void CharacterSnapshot::SetDashingDuration(const int dashing_duration) {
    dashing_duration_ = dashing_duration;
  }

  void CharacterSnapshot::Update(float dt) {
    snapshot_time_ += dt;
  }

  const float CharacterSnapshot::snapshot_time() {
    return snapshot_time_;
  }

  const std::vector<BulletPacket>& CharacterSnapshot::bullet_packets() {
    return bullet_packets_;
  }

  const Vector2& CharacterSnapshot::position() {
    return position_;
  }

  const int CharacterSnapshot::dashing_duration(){
    return dashing_duration_;
  }

  void CharacterSnapshot::Clear() {
    bullet_packets_.clear();
    snapshot_time_ = 0;
    dashing_duration_  = 0;
    position_ = Vector2::ZERO;
  }

}