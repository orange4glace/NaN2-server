// bullet_packet.cpp
#include "network/bullet_packet.h"

namespace nan2 {

  BulletPacket::BulletPacket(char type, const Vector2& position, unsigned char dir) : 
    type_(type),
    position_(position),
    dir_(dir) {
  }

  void BulletPacket::time(float time) {
    time_ = time;
  }

  const Vector2& BulletPacket::position() const {
    return position_;
  }

  float BulletPacket::time() const {
    return time_;
  }

}