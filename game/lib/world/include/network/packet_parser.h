#ifndef PACKET_PARSER_H_
#define PACKET_PARSER_H_

#include "player_input_packet.h"
#include "../flatbuffers/player_inputs_generated.h"

#include <flatbuffers/flatbuffers.h>

#include <vector>

#define fb nan2::game::world

namespace nan2 {

  class PacketParser {

  private:

  public:

    static std::vector<PlayerInputPacket> ParsePlayerInputPacket(uint8_t* buffer_pointer, int& out_player_id);

  };

}

#endif