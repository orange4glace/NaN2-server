// player.h
#ifndef WORLD_H_
#define WORLD_H_

#include "updatable.h"
#include "character.h"

#include "../network/client_snapshot_packet.h"
#include "../network/packet_state.h"

#include "../network/out_packet.h"

#include <queue>
#include <deque>

namespace nan2 {

  struct PingPacket {
    int seq;
    int time;
  };

  class Player : public Updatable {

  private:

    int id_;
    int ping_;
    std::queue<PingPacket> ping_queue_;
    Character character_;

    int packet_last_acked_;
    std::deque<PacketState> packet_states_;
    std::deque<OutPacket> packet_data_;

    int packet_seq_;

  public:

    Player(World* world, int id);

    const int id() const;
    int ping() const;
    Character& character();

    void ping(int val);

    void PushPingPacket(int seq, int time);
    void OnPongPacketReceived(int seq);
    void AddSnapshotPacket(ClientSnapshotPacket& client_snapshot_packet);

    void NotifySentPacket(const OutPacket& out_packet);
    int GetAndIncPacketSequence();

    int last_input_acked_packet() const;

  };
  
}

#endif