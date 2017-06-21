// out_packet.cpp
#include "network/out_packet.h"

namespace nan2 {

  int OutPacket::BROADCAST = 1;
  int OutPacket::UNICAST = 2;

  OutPacket::OutPacket(int seq, const std::vector<int8_t>& data, int type, uint64_t receiver) :
    seq_(seq),
    type_(type),
    receiver_(receiver) {
    data_.resize(data.size());
    std::copy(data.begin(), data.end(), data_.begin());
  }

  int OutPacket::seq() const {
    return seq_;
  }

  const std::vector<char>& OutPacket::data() const {
    return data_;
  }

  int OutPacket::type() const {
    return type_;
  }

  uint64_t OutPacket::receiver() const {
    return receiver_;
  }
}