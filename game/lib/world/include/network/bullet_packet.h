// bullet_packet.h
#ifndef BULLET_PACKET_H_
#define BULLET_PACKET_H_

#include <nan2/math/vector2.h>

namespace nan2 {

  class BulletPacket {

  private:

    float time_;
    char type_;
    Vector2 position_;
    unsigned char dir_;

  public:

    BulletPacket(char type, const Vector2& position, unsigned char dir);

    void time(float time);
    const Vector2& position() const;
    float time() const;

  };
  
}

#endif