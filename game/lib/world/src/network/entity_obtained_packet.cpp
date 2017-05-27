// entity_obtained_packet.cpp
#include "network/entity_obtained_packet.h"

#include "logger/logger.h"

namespace nan2 {

  EntityObtainedPacket::EntityObtainedPacket(Entity* entity) :
    entity_id_(entity->id()),
    entity_type_(entity->type()) {

  }

  entity_id EntityObtainedPacket::entity_id() const {
    return entity_id_;
  }

  entity_type EntityObtainedPacket::entity_type() const {
    return entity_type_;
  }

}