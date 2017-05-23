#ifndef PONG_PACKET_PARSER_H_
#define PONG_PACKET_PARSER_H_

#include "packet_parser.h"

namespace nan2 {

  class PongPacketParser : public PacketParser {

  private:

  public:

    PongPacketParser(int8_t* data, unsigned int size);

    int Parse(int& out_player_id);

  };

}

#endif