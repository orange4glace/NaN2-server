// player_input_packet_parser.cpp
#include "network/player_input_packet_parser.h"

#include "logger/logger.h"

namespace nan2 {

  PlayerInputPacketParser::PlayerInputPacketParser(int8_t* data, unsigned int size):
    PacketParser(data, size) {
    packet_type_ = PacketType::PLAYER_INPUT;
  }

  std::vector<PlayerInputPacket> PlayerInputPacketParser::Parse(int& out_player_id) {
    std::vector<PlayerInputPacket> ret;
    packet_type type = ReadInt();
    if (type != PacketType::PLAYER_INPUT) TypeMismatchException(type);

    int packet_size = ReadInt();
    int8_t* buffer = ReadBytes(packet_size);
    auto player_inputs = fb::GetPlayerInputs(buffer);
    out_player_id = player_inputs->player_id();
    auto player_inputs_vector = player_inputs->inputs();
    for (int i = 0; i < player_inputs_vector->Length(); i ++) {
      auto player_input = player_inputs_vector->Get(i);
      PlayerInputPacket input(player_input->sequence(), player_input->time(), player_input->move_dir(), player_input->fire_dir(), player_input->dash_dir());
      ret.push_back(input);
    }
    return ret;
  }

}
