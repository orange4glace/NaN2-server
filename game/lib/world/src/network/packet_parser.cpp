// packet_parser.cpp
#include "network/packet_parser.h"

#include "logger/logger.h"

#include <iostream>

namespace nan2 {

  std::vector<PlayerInputPacket> PacketParser::ParsePlayerInputPacket(uint8_t* buffer_pointer, int& out_player_id) {
    std::vector<PlayerInputPacket> ret;
    auto player_inputs = fb::GetPlayerInputs(buffer_pointer);
    out_player_id = player_inputs->id();
    auto player_inputs_vector = player_inputs->inputs();
    for (int i = 0; i < player_inputs_vector->Length(); i ++) {
      auto player_input = player_inputs_vector->Get(i);
      PlayerInputPacket input(player_input->time(), player_input->move_dir(), player_input->fire_dir());
      ret.push_back(input);
    }
    return ret;
  }

}