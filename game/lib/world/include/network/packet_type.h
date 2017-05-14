#ifndef PACKET_TYPE_H_
#define PACKET_TYPE_H_

typedef int packet_type;

namespace nan2 {

  namespace PacketType {

    const packet_type PING = 1;
    const packet_type PONG = 2;
    const packet_type SNAPSHOT = 3;
    const packet_type PLAYER_INPUT = 4;
  }
}

#endif