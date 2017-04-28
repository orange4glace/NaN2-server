// player_input_packet.cpp
#include "network/player_input_packet.h"

namespace nan2 {

  PlayerInputPacket::PlayerInputPacket(int time, unsigned char move_dir, unsigned char fire_dir) :
    time_(time / 1000.0f),
    move_dir_(move_dir),
    fire_dir_(fire_dir) {   
  }

  float PlayerInputPacket::Consume(float time) {
    is_consuming_ = true;
    if (time_ <= time) {
      float ret = time_;
      time_ = 0;
      return ret;
    }
    time_ -= time;
    return time;
  }

  const bool PlayerInputPacket::is_consuming() const {
      return is_consuming_;
  }

  const unsigned char PlayerInputPacket::move_dir() const {
      return move_dir_;
  }

  const unsigned char PlayerInputPacket::fire_dir() const {
      return fire_dir_;
  }

  const float PlayerInputPacket::time() const {
      return time_;
  }

  void PlayerInputPacket::Describe() const {
    L_DEBUG << move_dir_ << " " << fire_dir_ << " " << time_;
  }

}