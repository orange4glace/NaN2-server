#ifndef PACKET_BUILDER_H_
#define PACKET_BUILDER_H_

#include "packet_type.h"
#include <flatbuffers/flatbuffers.h>

#define fb nan2::game::world

namespace nan2 {

  class PacketBuilder {

  protected:

    PacketBuilder();
    ~PacketBuilder();

    int packet_type_;
    std::vector<int8_t> buffer_vector_;
    int8_t* buffer_array_;
    bool clean_;

    void AppendInt(uint32_t i);
    void AppendShort(uint16_t i);
    void AppendByte(uint8_t i);
    void AppendBytes(uint8_t* arr, int size);
    void AppendFlatBuffer(const flatbuffers::FlatBufferBuilder& builder);

  public:

    const int8_t* ToArray();
    const std::vector<int8_t>& GetBufferVector() const;
    void Clear();

  };

}

#endif