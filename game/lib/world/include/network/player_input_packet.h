// player_input_packet.h
#ifndef PLAYER_INPUT_PACKET_H_
#define PLAYER_INPUT_PACKET_H_

namespace nan2 {

  class PlayerInputPacket {

  private:

  	bool is_consuming_;
  	unsigned char move_dir_;
  	unsigned char fire_dir_;
  	int time_;

  public:

    PlayerInputPacket(unsigned char move_dir, unsigned char fire_dir, int time);

    int Consume(int time);

  	const unsigned char move_dir() const;
  	const unsigned char fire_dir() const;
  	const int time() const;
    const bool is_consuming() const;

  };
  
}

#endif