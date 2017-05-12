// bullet_packet.cpp
#include "network/bullet_packet.h"

namespace nan2 {

  BulletPacket::BulletPacket(int time, unsigned char type, unsigned char dir) : 
    time_(time),
    type_(type),
    dir_(dir) {
  }

  int BulletPacket::time() const {
    return time_;
  }

  unsigned char BulletPacket::type() const {
    return type_;
  }

  unsigned char BulletPacket::dir() const {
    return dir_;
  }

}