// pong_packet_parser.cpp
#include "network/pong_packet_parser.h"

#include "logger/logger.h"

namespace nan2 {

  PongPacketParser::PongPacketParser(int8_t* data, unsigned int size):
    PacketParser(data, size) {
    packet_type_ = PacketType::PONG;
  }

  int PongPacketParser::Parse(int& out_player_id) {
    packet_type type = ReadInt();
    if (type != PacketType::PONG) TypeMismatchException(type);

    out_player_id = ReadInt();
    int seq = ReadInt();
    return seq;
  }

}