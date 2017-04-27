//
// protocol.hpp
// header file defining packet_id and some constants
//

#ifndef _PROTOCOL_HPP_
#define _PROTOCOL_HPP_

const short   PACKET_JOIN_REQ        =   1;
const short   PACKET_JOIN_ANS        =   2;
const short  PACKET_GROUP_REQ        =   3;
const short  PACKET_GROUP_ANS        =   4;
const short  PACKET_GROUP_NTF        =   5;
const short  PACKET_MATCH_REQ        =   6;
const short  PACKET_MATCH_ANS        =   7; 
const short  PACKET_MATCH_NTF        =   8;

namespace nan2 {
enum GameMode {
  DEATH
};
}

#endif
