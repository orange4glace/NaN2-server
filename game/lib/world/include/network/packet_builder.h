#ifndef PACKET_BUILDER_H_
#define PACKET_BUILDER_H_

#include "../world/world.h"

#include "../flatbuffers/world_generated.h"

#include <flatbuffers/flatbuffers.h>
#include <vector>

#define fb nan2::game::world

namespace nan2 {

  class PacketBuilder {

  private:

  public:

    static uint8_t* BuildPacket(World& world);

  };

}

#endif