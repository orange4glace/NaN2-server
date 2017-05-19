// entity_created_packet.h
#ifndef ENTITY_CREATED_PACKET_H_
#define ENTITY_CREATED_PACKET_H_

#include "../entity/entity.h"

namespace nan2 {

  class EntityCreatedPacket {

  private:

    uint16_t entity_id_;
    uint8_t entity_type_;
    Vector2 position_;

  public:

    EntityCreatedPacket(Entity* entity);

    uint16_t entity_id() const;
    uint8_t entity_type() const;
    const Vector2& position() const;

  };

}

#endif