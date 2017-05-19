#ifndef PLAYER_INPUT_PACKET_PARSER_H_
#define PLAYER_INPUT_PACKET_PARSER_H_

#include "packet_parser.h"
#include "player_input_packet.h"

#include <vector>

namespace nan2 {

  class PlayerInputPacketParser : public PacketParser {

  private:

  public:

    PlayerInputPacketParser(uint8_t* data, unsigned int size);

    std::vector<PlayerInputPacket> Parse(int& out_player_id);

  };

}

#endif