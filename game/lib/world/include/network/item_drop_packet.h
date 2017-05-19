#ifndef ITEM_DROP_PACKET_H_
#define ITEM_DROP_PACKET_H_

namespace nan2 {

  class ItemDropPacket {

  private:

    int item_id_;
    Vector2 position_;

  public:

    ItemDropPacket(int item_id, const Vector2& position);

    int item_id() const;
    const Vector2& position() const;

  };

}

#endif