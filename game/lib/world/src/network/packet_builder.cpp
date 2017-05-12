// packet_parser.cpp
#include "network/packet_builder.h"

#include "logger/logger.h"

#include <vector>

namespace nan2 {

  // Protected initializer
  PacketBuilder::PacketBuilder() :
    buffer_array_(nullptr),
    packet_type_(0) {
  }

  PacketBuilder::~PacketBuilder() {
    if (buffer_array_ != nullptr) delete buffer_array_;
  }

  const uint8_t* PacketBuilder::ToArray() {
    if (buffer_array_ != nullptr) delete buffer_array_;
    buffer_array_ = new uint8_t[buffer_vector_.size()];
    std::copy(buffer_vector_.begin(), buffer_vector_.end(), buffer_array_);
    return buffer_array_;
  }

  const std::vector<uint8_t>& PacketBuilder::GetBufferVector() const {
    return buffer_vector_;
  }

  void PacketBuilder::AppendInt(uint32_t i) {
    buffer_vector_.push_back((i) & 0xFF);
    buffer_vector_.push_back((i >> 8) & 0xFF);
    buffer_vector_.push_back((i >> 16) & 0xFF);
    buffer_vector_.push_back((i >> 24) & 0xFF);
  }

  void PacketBuilder::AppendShort(uint16_t i) {
    buffer_vector_.push_back((i) & 0xFF);
    buffer_vector_.push_back((i >> 8) & 0xFF);
  }

  void PacketBuilder::AppendByte(uint8_t i) {
    buffer_vector_.push_back(i);
  }

  void PacketBuilder::AppendBytes(uint8_t* arr, int size) {
    int st = buffer_vector_.size();
    buffer_vector_.resize(buffer_vector_.size() + size);
    std::copy(arr, arr + size, buffer_vector_.begin() + st);
  }

  void PacketBuilder::AppendFlatBuffer(const flatbuffers::FlatBufferBuilder& builder_) {
    AppendInt(builder_.GetSize());
    AppendBytes(builder_.GetBufferPointer(), builder_.GetSize());
  }

  void PacketBuilder::Clear() {
    buffer_vector_.clear();
    delete buffer_array_;
    clean_ = true;
  }

}