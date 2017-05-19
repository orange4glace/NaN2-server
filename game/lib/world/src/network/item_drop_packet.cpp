// item_drop_packet.cpp
#include "network/item_drop_packet.h"

#include "logger/logger.h"

namespace nan2 {

  ItemDropPacket::ItemDropPacket(int item_id, const Vector2& position) :
    item_id_(item_id),
    position_(position) {

  }

  int ItemDropPacket::item_id() const {
    return item_id_;
  }

  const Vector2& ItemDropPacket::position() const {
    return position_;
  }

}