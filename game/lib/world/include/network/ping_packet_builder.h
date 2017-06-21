#ifndef PING_PACKET_BUILDER_H_
#define PING_PACKET_BUILDER_H_

#include "packet_builder.h"

namespace nan2 {

  class PingPacketBuilder : public PacketBuilder {

  private:

  public:

    PingPacketBuilder();

    int Build(int seq, int ping);

  };

}

#endif