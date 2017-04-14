// tick_packet.h
#ifndef TICK_PACKET_H_
#define TICK_PACKET_H_

namespace nan2 {

  class TickPacket {

  private:

    float time;

  public:

    TickPacket();

    float Consume(float time);

  };
  
}

#endif