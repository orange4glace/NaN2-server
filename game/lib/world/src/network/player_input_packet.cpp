// player_input_packet.cpp
#include "network/player_input_packet.h"

#include "logger/logger.h"

namespace nan2 {

  PlayerInputPacket::PlayerInputPacket(unsigned int sequence, int time, unsigned char move_dir, unsigned char fire_dir,
    unsigned char dash_dir, unsigned char view_dir) :
  sequence_(sequence),
  time_(time),
  move_dir_(move_dir),
  fire_dir_(fire_dir),
  dash_dir_(dash_dir),
  view_dir_(view_dir) {   
  }

  int PlayerInputPacket::Consume(int time) {
    is_consuming_ = true;
    if (time_ <= time) {
      int ret = time_;
      time_ = 0;
      return ret;
    }
    time_ -= time;
    return time;
  }

  const bool PlayerInputPacket::is_consuming() const {
    return is_consuming_;
  }

  const unsigned int PlayerInputPacket::sequence() const {
    return sequence_;
  }

  const unsigned char PlayerInputPacket::move_dir() const {
    return move_dir_;
  }

  const unsigned char PlayerInputPacket::fire_dir() const {
    return fire_dir_;
  }

  const unsigned char PlayerInputPacket::dash_dir() const {
    return dash_dir_;
  }

  const unsigned char PlayerInputPacket::view_dir() const {
    return view_dir_;
  }

  const bool PlayerInputPacket::obtaining() const {
    return obtaining_;
  }

  const int PlayerInputPacket::time() const {
    return time_;
  }

  void PlayerInputPacket::Describe() const {
    L_DEBUG << move_dir_ << " " << fire_dir_ << " " << time_;
  }

}