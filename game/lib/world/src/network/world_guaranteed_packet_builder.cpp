// world_guaranteed_packet_builder.cpp
#include "network/world_guaranteed_packet_builder.h"

#include "logger/logger.h"

#include "flatbuffers/entity_created_generated.h"

namespace nan2 {

  WorldGuaranteedPacketBuilder::WorldGuaranteedPacketBuilder() :
    PacketBuilder() {
    packet_type_ = PacketType::WORLD_GUARANTEED;
  }

  void WorldGuaranteedPacketBuilder::AddEntityCreated(Entity* entity) {
    entity_created_.push(EntityCreatedPacket(entity));
  }

  void WorldGuaranteedPacketBuilder::AddEntityDestroied(Entity* entity) {
    entity_destroied_.push(EntityDestroiedPacket(entity));
  }

  void WorldGuaranteedPacketBuilder::Build(World& world) {
    if (!clean_) Clear();
    clean_ = false;

    AppendInt(PacketType::WORLD_GUARANTEED);

    flatbuffers::FlatBufferBuilder builder_;

    std::vector<flatbuffers::Offset<fb::EntityCreated>> entity_created_vector;
    std::vector<flatbuffers::Offset<uint16_t>> entity_destroied_vector;

  }

  void WorldGuaranteedPacketBuilder::Clear() {
    PacketBuilder::Clear();
    if (entity_created_.size() > 0) {
      std::queue<EntityCreatedPacket> tmp;
      std::swap(entity_created_, tmp);
    }
    if (entity_destroied_.size() > 0) {
      std::queue<EntityDestroiedPacket> tmp;
      std::swap(entity_destroied_, tmp);
    }
  }

}
