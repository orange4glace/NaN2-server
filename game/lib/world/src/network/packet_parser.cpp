// packet_parser.cpp
#include "network/packet_parser.h"

#include "logger/logger.h"

#include <cstring>
#include <string>
#include <iostream>

namespace nan2 {

  PacketParser::PacketParser(int8_t* data, unsigned int size):
    size_(size) {
    data_= new int8_t[size];
    std::memcpy(data_, data, size);
  }

  packet_type PacketParser::GetPacketType(int8_t* data) {
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

  int32_t PacketParser::ReadInt() {
    Take(4);
    int32_t result = 0;
    result = ((uint8_t)data_[offset_ - 1] << 24) | ((uint8_t)data_[offset_ - 2] << 16) | ((uint8_t)data_[offset_ - 3] << 8) | ((uint8_t)data_[offset_ - 4]);
    return result;
  }

  int16_t PacketParser::ReadShort() {
    Take(2);
    int16_t result = 0;
    result = ((uint8_t)data_[offset_ - 1] << 8) | ((uint8_t)data_[offset_ - 2]);
    return result;
  }

  int8_t PacketParser::ReadByte() {
    Take(1);
    int8_t result = 0;
    result = ((uint8_t)data_[offset_ - 1]);
    return result;
  }

  int8_t* PacketParser::ReadBytes(unsigned int size) {
    Take(size);
    int8_t* result = new int8_t[size];
    std::memcpy(result, data_ + offset_ - size, size);
    return result;
  }

  void PacketParser::Take(unsigned int size) {
    if (size_ < offset_ + size) SizeOverflowException();
    offset_ += size;
  }

  void PacketParser::PrintRaw() {
    for (int i = 0; i < size_; i ++)
      std::cout << (int)((unsigned char)data_[i]) << " ";
    std::cout << "\n";
  }
}