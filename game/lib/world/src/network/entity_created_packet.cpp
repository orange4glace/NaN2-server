// entity_created_packet.cpp
#include "network/entity_created_packet.h"

#include "logger/logger.h"

namespace nan2 {

  EntityCreatedPacket::EntityCreatedPacket(Entity* entity) :
    entity_id_(entity->id()),
    entity_type_(entity->type()),
    position_(entity->position()) {

  }

  entity_id EntityCreatedPacket::entity_id() const {
    return entity_id_;
  }

  entity_type EntityCreatedPacket::entity_type() const {
    return entity_type_;
  }

  const Vector2& ItemDropPacket::position() const {
    return position_;
  }

}