// ping_packet_builder.cpp
#include "network/ping_packet_builder.h"

#include "logger/logger.h"

namespace nan2 {

  PingPacketBuilder::PingPacketBuilder() :
    PacketBuilder() {
    packet_type_ = PacketType::PING;
  }

  void PingPacketBuilder::Build(int seq, int ping) {
    if (!clean_) Clear();
    clean_ = false;
    AppendInt(PacketType::PING);
    AppendInt(seq);
    AppendInt(ping);
  }

}