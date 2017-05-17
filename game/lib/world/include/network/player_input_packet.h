#ifndef PLAYER_INPUT_PACKET_H_
#define PLAYER_INPUT_PACKET_H_

namespace nan2 {

  class PlayerInputPacket {

  private:

    bool is_consuming_;
    unsigned int sequence_;
    unsigned char move_dir_;
    unsigned char fire_dir_;
    unsigned char dash_dir_;
    int time_;

  public:

    PlayerInputPacket(unsigned int sequence, int time, unsigned char move_dir, unsigned char fire_dir, unsigned char dash_dir);

    int Consume(int time);

    const unsigned int sequence() const;
    const unsigned char move_dir() const;
    const unsigned char fire_dir() const;
    const unsigned char dash_dir() const;
    const int time() const;
    const bool is_consuming() const;

    void Describe() const;

  };

}

#endif