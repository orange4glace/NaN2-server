// OUT_PACKET.h
#ifndef OUT_PACKET_H_
#define OUT_PACKET_H_

#include <cstdint>
#include <vector>

namespace nan2 {

  class OutPacket {

  private:

    std::vector<char> data_;
    int type_;
    uint64_t receiver_;


  public:

    static int BROADCAST;
    static int UNICAST;

    OutPacket(const std::vector<int8_t>& data, int type, uint64_t receiver = 0);

    const std::vector<char>& data() const;
    int type() const;
    uint64_t receiver() const;

  };
  
}

#endif