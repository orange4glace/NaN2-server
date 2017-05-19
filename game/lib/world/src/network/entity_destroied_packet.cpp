// entity_destroied_packet.cpp
#include "network/entity_destroied_packet.h"

#include "logger/logger.h"

namespace nan2 {

  EntityDestroiedPacket::EntityDestroiedPacket(Entity* entity) :
    entity_id_(entity->id()) {

  }

  entity_id EntityDestroiedPacket::entity_id() const {
    return entity_id_;
  }

}