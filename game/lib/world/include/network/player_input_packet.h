// player_input_packet.h
#ifndef PLAYER_INPUT_PACKET_H_
#define PLAYER_INPUT_PACKET_H_

#include "../logger/logger.h"

namespace nan2 {

  class PlayerInputPacket {

  private:

  	bool is_consuming_;
  	unsigned char move_dir_;
  	unsigned char fire_dir_;
  	float time_;

  public:

    PlayerInputPacket(int time, unsigned char move_dir, unsigned char fire_dir);

    float Consume(float time);

  	const unsigned char move_dir() const;
  	const unsigned char fire_dir() const;
  	const float time() const;
    const bool is_consuming() const;

    void Describe() const;

  };
  
}

#endif