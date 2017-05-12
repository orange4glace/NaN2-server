// packet_parser.cpp
#include "network/packet_builder.h"

#include "logger/logger.h"

#include <vector>

namespace nan2 {

  // Protected initializer
  PacketBuilder::PacketBuilder(unsigned int size) :
    builder_(size) {
  }

  void PacketBuilder::BuildBufferVector() {
    buffer_vector_.resize(builder_.GetSize());
    std::copy(builder_.GetBufferPointer(), builder_.GetBufferPointer() + builder_.GetSize(), buffer_vector_.begin());
  }

  const std::vector<uint8_t>& PacketBuilder::GetBufferVector() const {
    return buffer_vector_;
  }

  void PacketBuilder::Clear() {
    builder_.Clear();
    buffer_vector_.clear();
    clean_ = true;
  }

}