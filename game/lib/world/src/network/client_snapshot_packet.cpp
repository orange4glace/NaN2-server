// client_snapshot_packet.cpp
#include "network/client_snapshot_packet.h"

#include "logger/logger.h"

namespace nan2 {

  ClientSnapshotPacket::ClientSnapshotPacket(int packet_last_acked, int packets_acked_bits) :
    packet_last_acked_(packet_last_acked),
    packets_acked_bits_(packets_acked_bits) {

  }

  void ClientSnapshotPacket::AddPlayerInput(PlayerInputPacket& pip) {
    player_inputs_.push_back(pip);
  }

  PlayerInputPacket ClientSnapshotPacket::GetPlayerInput(int i) {
    return player_inputs_[i];
  }

  int ClientSnapshotPacket::GetPlayerInputsSize() const {
    return player_inputs_.size();
  }

  int ClientSnapshotPacket::packet_last_acked() const {
    return packet_last_acked_;
  }

  int ClientSnapshotPacket::packets_acked_bits() const {
    return packets_acked_bits_;
  }
}