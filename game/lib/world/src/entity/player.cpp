// player.cpp
#include "entity/player.h"

#include "logger/logger.h"

namespace nan2 {
  
  Player::Player(World* world, int id) :
    Updatable(world, Entity::GROUP_PLAYER, Entity::TYPE_PLAYER),
    character_(this),
    id_(id) {
    ping_ = 0;
  }

  const int Player::id() const {
    return id_;
  }

  int Player::ping() const {
    return ping_;
  }

  Character& Player::character() {
    return character_;
  }

  void Player::ping(int val) {
    if (abs(ping_ - val) < 5) {
      ping_ = val;
      return;
    }
    int v = (val - ping_) / 10;
    ping_ += v;
  }

  void Player::PushPingPacket(int seq, int time) {
    PingPacket ping;
    ping.seq = seq;
    ping.time = time;
    ping_queue_.push(ping);
  }

  void Player::OnPongPacketReceived(int seq) {
    L_DEBUG << "handle " << seq << " " << id_;
    while (!ping_queue_.empty()) {
      PingPacket ping_packet = ping_queue_.front();
      L_DEBUG << "chk " << ping_packet.seq;
      if (ping_packet.seq < seq) {
        ping_queue_.pop();
        continue;
      }
      else if (ping_packet.seq > seq) break;
      else {
        int rtt = Time::current_time() - ping_packet.time;
        ping(rtt);
        ping_queue_.pop();
      }
    }
  }

  void Player::AddSnapshotPacket(ClientSnapshotPacket& client_snapshot_packet) {
    for (int i = 0; i < client_snapshot_packet.GetPlayerInputsSize(); i ++)
      character_.AddInput(client_snapshot_packet.GetPlayerInput(i));
  }

  int Player::last_input_acked_packet() const {
    return character_.last_input_acked_packet_;
  }

}