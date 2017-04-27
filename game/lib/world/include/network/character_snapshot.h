// character_snapshot.h
#ifndef CHARACTER_SNAPSHOT_H_
#define CHARACTER_SNAPSHOT_H_

#include "bullet_packet.h"
#include <nan2/math/vector2.h>

#include <vector>

namespace nan2 {

  class CharacterSnapshot {

  private:

    float snapshot_time_;
    std::vector<BulletPacket> bullet_packets_;
    Vector2 position_;
    int dashing_duration_;

  public:

    CharacterSnapshot();

    void Update(float dt);
    void AddBulletPacket(BulletPacket& bullet);
    void SetPosition(const Vector2& position);
    void SetDashingDuration(const int dashing_duration);

    const float snapshot_time();
    const std::vector<BulletPacket>& bullet_packets();
    const Vector2& position();
    const int dashing_duration();

    void Clear();


  };
  
}

#endif