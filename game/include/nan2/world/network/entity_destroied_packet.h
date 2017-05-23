// entity_destroied_packet.h
#ifndef ENTITY_DESTROIED_PACKET_H_
#define ENTITY_DESTROIED_PACKET_H_

#include "../entity/entity.h"

namespace nan2 {

  class EntityDestroiedPacket {

  private:

    uint16_t entity_id_;

  public:

    EntityDestroiedPacket(Entity* entity);

    uint16_t entity_id() const;

  };

}

#endif