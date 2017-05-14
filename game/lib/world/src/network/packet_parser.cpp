// packet_parser.cpp
#include "network/packet_parser.h"

#include "logger/logger.h"

#include <cstring>
#include <iostream>

namespace nan2 {

  PacketParser::PacketParser(uint8_t* data, unsigned int size):
    size_(size) {
    data_= new uint8_t[size];
    std::memcpy(data_, data, size);
  }

  packet_type PacketParser::GetPacketType(uint8_t* data) {
    int type = (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | (data[0]);
    switch (type) {
      case PacketType::PING:
        return PacketType::PING;
      case PacketType::PONG:
        return PacketType::PONG;
      case PacketType::SNAPSHOT:
        return PacketType::SNAPSHOT;
      case PacketType::PLAYER_INPUT:
        return PacketType::PLAYER_INPUT;  
    }
    return 0;
  }

  uint32_t PacketParser::ReadInt() {
    Take(4);
    uint32_t result = 0;
    result = (data_[offset_ - 1] << 24) | (data_[offset_ - 2] << 16) | (data_[offset_ - 3] << 8) | (data_[offset_ - 4]);
    return result;
  }

  uint16_t PacketParser::ReadShort() {
    Take(2);
    uint16_t result = 0;
    result = (data_[offset_ - 1] << 8) | (data_[offset_ - 2]);
    return result;
  }

  uint8_t PacketParser::ReadByte() {
    Take(1);
    uint8_t result = 0;
    result = (data_[offset_ - 1]);
    return result;
  }

  uint8_t* PacketParser::ReadBytes(unsigned int size) {
    Take(size);
    uint8_t* result = new uint8_t[size];
    std::memcpy(result, data_ + offset_ - size, size);
    return result;
  }

  void PacketParser::Take(unsigned int size) {
    if (size_ < offset_ + size) SizeOverflowException();
    offset_ += size;
  }

}