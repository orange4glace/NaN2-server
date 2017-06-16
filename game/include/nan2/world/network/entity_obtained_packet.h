// entity_obtained_packet.h
#ifndef ENTITY_ObtainED_PACKET_H_
#define ENTITY_ObtainED_PACKET_H_

#include "../entity/entity.h"

namespace nan2 {

  class EntityObtainedPacket {

  private:

    uint16_t entity_id_;
    uint8_t entity_type_;

  public:

    EntityObtainedPacket(Entity* entity);

    uint16_t entity_id() const;
    uint8_t entity_type() const;

  };

}

#endif