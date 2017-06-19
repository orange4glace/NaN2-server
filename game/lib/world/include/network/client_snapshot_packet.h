#ifndef CLIENT_SNAPSHOT_PACKET_H_
#define CLIENT_SNAPSHOT_PACKET_H_

#include "player_input_packet.h"

#include <vector>

namespace nan2 {

  class ClientSnapshotPacket {

  private:

    std::vector<PlayerInputPacket> player_inputs_;
    int packet_last_acked_;
    int packets_acked_bits_;

  public:

    ClientSnapshotPacket(int pakcet_last_acked, int packets_acked_bits);

    void AddPlayerInput(PlayerInputPacket& pip);
    PlayerInputPacket GetPlayerInput(int i);
    
    int GetPlayerInputsSize() const;
    int packet_last_acked() const;
    int packets_acked_bits() const;

  };

}

#endif