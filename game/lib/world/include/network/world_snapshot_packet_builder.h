#ifndef WORLD_SNAPSHOT_PACKET_BUILDER_H_
#define WORLD_SNAPSHOT_PACKET_BUILDER_H_

#include "packet_builder.h"
#include "../world/world.h"

#include "../flatbuffers/world_generated.h"

namespace nan2 {

  class WorldSnapshotPacketBuilder : public PacketBuilder {

  private:

  public:

    WorldSnapshotPacketBuilder();

    void Build(World& world);

  };

}

#endif