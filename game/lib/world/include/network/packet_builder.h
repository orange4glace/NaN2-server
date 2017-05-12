#ifndef PACKET_BUILDER_H_
#define PACKET_BUILDER_H_

#include <flatbuffers/flatbuffers.h>

#define fb nan2::game::world

namespace nan2 {

  class PacketBuilder {

  protected:

    PacketBuilder(unsigned int size);

    void BuildBufferVector();

    flatbuffers::FlatBufferBuilder builder_;
    std::vector<uint8_t> buffer_vector_;
    bool clean_;

  public:

    const std::vector<uint8_t>& GetBufferVector() const;
    void Clear();

  };

}

#endif