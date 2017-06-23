#ifndef PLAYER_INPUT_PACKET_PARSER_H_
#define PLAYER_INPUT_PACKET_PARSER_H_

#include "packet_parser.h"
#include "client_snapshot_packet.h"

#include <vector>

namespace nan2 {

  class ClientSnapshotPacketParser : public PacketParser {

  private:

  public:

    ClientSnapshotPacketParser(int8_t* data, unsigned int size);

    ClientSnapshotPacket Parse(int& out_player_id);

  };

}

#endif