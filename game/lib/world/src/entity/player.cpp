// player.cpp
#include "entity/player.h"

namespace nan2 {
  
  Player::Player(World* world, int id) :
    Updatable(world),
    character_(this),
    id_(id) {

  }

  const int Player::id() const {
    return id_;
  }

  Character& Player::character() {
    return character_;
  }

  int Player::last_input_acked_packet() const {
    return character_.last_input_acked_packet_;
  }

}