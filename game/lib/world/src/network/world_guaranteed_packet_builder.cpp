// world_guaranteed_packet_builder.cpp
#include "network/world_guaranteed_packet_builder.h"

#include "logger/logger.h"

#include "flatbuffers/world_guaranteed_generated.h"
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

  void WorldGuaranteedPacketBuilder::Build() {
    if (!clean_) Clear();
    clean_ = false;

    AppendInt(5);

    flatbuffers::FlatBufferBuilder builder;

    std::vector<flatbuffers::Offset<fb::EntityCreated>> entities_created_vector;
    std::vector<uint16_t> entities_destroied_vector;

    while (entity_created_.size()) {
      const EntityCreatedPacket& packet = entity_created_.front();
      fb::EntityCreatedBuilder entity_created_builder(builder);
      entity_created_builder.add_id(packet.entity_id());
      entity_created_builder.add_type(packet.entity_type());
      fb::Vec2 pos(packet.position().x(), packet.position().y());
      entity_created_builder.add_position(&pos);
      auto offset = entity_created_builder.Finish();
      entities_created_vector.push_back(offset);
      entity_created_.pop();
    }
    auto entities_created = builder.CreateVector(entities_created_vector);

    while (entity_destroied_.size()) {
      const EntityDestroiedPacket& packet = entity_destroied_.front();
      entities_destroied_vector.push_back(packet.entity_id());
      entity_destroied_.pop();
    }
    auto entities_destroied = builder.CreateVector(entities_destroied_vector);

    fb::WorldGuaranteedBuilder world_guaranteed_builder(builder);
    world_guaranteed_builder.add_entities_created(entities_created);
    world_guaranteed_builder.add_entities_destroied(entities_destroied);

    auto world_guaranteed_offset = world_guaranteed_builder.Finish();

    builder.Finish(world_guaranteed_offset);

    AppendFlatBuffer(builder);
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