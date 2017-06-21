// packet_state.cpp
#include "network/packet_state.h"

namespace nan2 {

  PacketState::PacketState(int seq, int origin_seq, int retries) : 
    seq_(seq),
    origin_seq_(origin_seq),
    retries_(retries) {
  }

  int PacketState::seq() const {
    return seq_;
  }

  int PacketState::origin_seq() const {
    return origin_seq_;
  }

  int PacketState::retries() const {
    return retries_;
  }

  bool PacketState::is_origin() const {
    return seq_ == origin_seq_;
  }

}