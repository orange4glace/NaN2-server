#ifndef PACKET_PARSER_H_
#define PACKET_PARSER_H_

#include "player_input_packet.h"
#include "packet_type.h"
#include "../flatbuffers/player_inputs_generated.h"

#include <flatbuffers/flatbuffers.h>

#include <exception>
#include <vector>

#define fb nan2::game::world

namespace nan2 {

  class PacketParser {

  protected:

    int8_t* data_;
    unsigned int size_;
    unsigned int offset_ = 0;
    packet_type packet_type_ = 0;

    PacketParser(int8_t* data, unsigned int size);

    int32_t ReadInt();
    int16_t ReadShort();
    int8_t  ReadByte();
    int8_t* ReadBytes(unsigned int size);

    void Take(unsigned int size);

    inline const void TypeMismatchException(packet_type received) {
      throw "Packet type mismatch";
    }
    inline const void SizeOverflowException() {
      throw "Packet size overflow";
    }
    inline const void SizeUnderflowException() {
      throw "Packet size underflow";
    }

  public:

    static packet_type GetPacketType(int8_t* data);
    void Clear();
    void PrintRaw();

  };

}

#endif