// player.h
#ifndef WORLD_H_
#define WORLD_H_

#include "updatable.h"
#include "character.h"

#include <queue>

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

  public:

    Player(World* world, int id);

    const int id() const;
    int ping() const;
    Character& character();

    void ping(int val);

    void PushPingPacket(int seq, int time);
    void OnPongPacketReceived(int seq);

    int last_input_acked_packet() const;

  };
  
}

#endif