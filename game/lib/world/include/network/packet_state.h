// packet_state.h
#ifndef PACKET_STATE_H_
#define PACKET_STATE_H_

namespace nan2 {

  class PacketState {

  private:

    int seq_;
    int origin_seq_;
    int retries_;

  public:

    PacketState(int seq, int origin_seq, int retries);

    int seq() const;
    int origin_seq() const;
    bool is_origin() const;
    int retries() const;

  };
  
}

#endif