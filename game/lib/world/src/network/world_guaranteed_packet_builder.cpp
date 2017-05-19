// world_guaranteed_packet_builder.cpp
#include "network/world_guaranteed_packet_builder.h"

#include "logger/logger.h"

namespace nan2 {

  WorldGuaranteedPacketBuilder::WorldGuaranteedPacketBuilder() :
    PacketBuilder() {
    packet_type_ = PacketType::GUARANTEED_PACKET;
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
  }

  void WorldGuaranteedPacketBuilder::Clear() {
    PacketBuilder::Clear();
    entity_created_.clear();
    entity_destroied_.clear();
  }

}
