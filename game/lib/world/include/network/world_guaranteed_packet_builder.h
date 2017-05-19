#ifndef WORLD_GUARANTEED_PACKET_BUILDER_H_
#define WORLD_GUARANTEED_PACKET_BUILDER_H_

#include "packet_builder.h"

#include "../world/world.h"
#include "entity_created_packet.h"
#include "entity_destroied_packet.h"

#include "../flatbuffers/world_generated.h"

#include <vector>
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

    void Build(World& world);
    void Clear();

  };

}

#endif