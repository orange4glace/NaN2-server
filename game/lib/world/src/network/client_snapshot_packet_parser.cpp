// player_input_packet_parser.cpp
#include "network/client_snapshot_packet_parser.h"

#include "logger/logger.h"

namespace nan2 {

  ClientSnapshotPacketParser::ClientSnapshotPacketParser(int8_t* data, unsigned int size):
    PacketParser(data, size) {
    packet_type_ = PacketType::PLAYER_INPUT;
  }

  ClientSnapshotPacket ClientSnapshotPacketParser::Parse(int& out_player_id) {
    packet_type type = ReadInt();
    if (type != PacketType::PLAYER_INPUT) TypeMismatchException(type);

    int packet_last_acked = ReadInt();
    int packets_acked_bits = ReadInt();

    ClientSnapshotPacket ret(packet_last_acked, packets_acked_bits);

    int packet_size = ReadInt();
    int8_t* buffer = ReadBytes(packet_size);
    auto player_inputs = fb::GetPlayerInputs(buffer);
    out_player_id = player_inputs->player_id();
    auto player_inputs_vector = player_inputs->inputs();
    for (int i = 0; i < player_inputs_vector->Length(); i ++) {
      auto player_input = player_inputs_vector->Get(i);
      PlayerInputPacket input(player_input->sequence(), player_input->time(), player_input->move_dir(), player_input->fire_dir(), player_input->dash_dir());
      ret.AddPlayerInput(input);
    }
    return ret;
  }

}