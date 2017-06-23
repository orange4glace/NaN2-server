#ifndef WORLD_GUARANTEED_PACKET_BUILDER_H_
#define WORLD_GUARANTEED_PACKET_BUILDER_H_

#include "packet_builder.h"

#include "../flatbuffers/world_guaranteed_generated.h"
#include "entity_created_packet.h"
#include "entity_destroied_packet.h"

#include <vector>
#include <queue>
#include <utility>

namespace nan2 {

  class WorldGuaranteedPacketBuilder : public PacketBuilder {

  private:

    std::queue<EntityCreatedPacket> entity_created_;
    std::queue<EntityDestroiedPacket> entity_destroied_;

  public:

    WorldGuaranteedPacketBuilder();

    void AddEntityCreated(Entity* entity);
    void AddEntityDestroied(Entity* entity);

    int Build();
    void Clear();

  };

}

#endif