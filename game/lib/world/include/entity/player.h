// player.h
#ifndef WORLD_H_
#define WORLD_H_

#include "updatable.h"
#include "character.h"

namespace nan2 {

  class Player : public Updatable {

  private:

    int id_;
    Character character_;

  public:

    Player(World* world, int id);

    const int id() const;
    Character& character();

    int last_input_acked_packet() const;

  };
  
}

#endif