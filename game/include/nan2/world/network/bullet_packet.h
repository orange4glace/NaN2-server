// bullet_packet.h
#ifndef BULLET_PACKET_H_
#define BULLET_PACKET_H_

#include <nan2/math/vector2.h>

namespace nan2 {

  class BulletPacket {

  private:

    int time_;
    unsigned char type_;
    unsigned char dir_;

  public:

    BulletPacket(int time, unsigned char type, unsigned char dir);

    int time() const;
    unsigned char type() const;
    unsigned char dir() const;

  };
  
}

#endif