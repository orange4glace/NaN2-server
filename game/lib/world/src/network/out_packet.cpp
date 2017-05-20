// out_packet.cpp
#include "network/out_packet.h"

namespace nan2 {

  int OutPacket::BROADCAST = 1;
  int OutPacket::UNICAST = 2;

  OutPacket::OutPacket(const std::vector<int8_t>& data, int type, uint64_t receiver) :
    data_(data),
    type_(type),
    receiver_(receiver) {
  }

  const std::vector<int8_t>& OutPacket::data() const {
    return data_;
  }

  int OutPacket::type() const {
    return type_;
  }

  uint64_t OutPacket::receiver() const {
    return receiver_;
  }
}