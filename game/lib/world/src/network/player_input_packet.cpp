// player_input_packet.cpp
#include "network/player_input_packet.h"

namespace nan2 {

  const bool PlayerInputPacket::is_consuming() const {
      return is_consuming_;
  }

  const unsigned char PlayerInputPacket::move_dir() const {
      return move_dir_;
  }

  const unsigned char PlayerInputPacket::fire_dir() const {
      return fire_dir_;
  }

  const int PlayerInputPacket::time() const {
      return time_;
  }

}