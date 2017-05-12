#include <iostream>
#include <string>

#include <mgne/protocol.hpp>
#include <boost/asio.hpp>

#include "../include/join_req_generated.h"
#include "../include/join_ans_generated.h"
#include "../include/group_req_generated.h"
#include "../include/group_ntf_generated.h"
#include "../include/group_ans_generated.h"

#include "../include/protocol.hpp"

using namespace nan2::lobby;

const char SERVER_IP[] = "127.0.0.1";
const unsigned short PORT_NUMBER = 4000;

void write_(boost::asio::ip::tcp::socket& socket, char* buffer, int len)
{
  boost::asio::write(socket, boost::asio::buffer(buffer, len));
}

int read_(boost::asio::ip::tcp::socket& socket, char* buffer, int len)
{
  return boost::asio::read(socket, boost::asio::buffer(buffer, len));
}

int main()
{
  boost::asio::io_service io_service;
  boost::asio::ip::tcp::endpoint endpoint(
    boost::asio::ip::address::from_string(SERVER_IP), PORT_NUMBER);
  boost::asio::ip::tcp::socket socket(io_service);

  socket.connect(endpoint);

  int ntf_id;
  int len;
  int packet_id;
  char buffer[1024];

  TCP_PACKET_HEADER header;
  flatbuffers::FlatBufferBuilder builder(1024);

  /* 1. join */
  std::string token_str = "wv9TDmj8bO@n%7Aff^s9";
  auto token = builder.CreateString(token_str);
  auto join_req = CreateJoinReq(builder, token);
  builder.Finish(join_req);

  header.packet_size = sizeof(TCP_PACKET_HEADER) + builder.GetSize();
  header.packet_id = PACKET_JOIN_REQ;

  write_(socket, (char*)&header, sizeof(TCP_PACKET_HEADER));
  write_(socket, (char*)builder.GetBufferPointer(), builder.GetSize());

  std::cout << "Sended    : " << builder.GetSize() << std::endl;

  /* 2. receive join_ans */
  len = read_(socket, buffer, sizeof(TCP_PACKET_HEADER));
  std::cout << "Received  : " << len << std::endl;
  std::cout << "Packet id : " << ((TCP_PACKET_HEADER*)buffer)->packet_id
    << std::endl;
  len = read_(socket, buffer,
    ((TCP_PACKET_HEADER*)buffer)->packet_size - sizeof(TCP_PACKET_HEADER));

  /* 3. receive group join_rq ntf */
  len = read_(socket, buffer, sizeof(TCP_PACKET_HEADER));
  packet_id = ((TCP_PACKET_HEADER*)buffer)->packet_id;
  std::cout << "Packet id : " << packet_id << std::endl;
  len = read_(socket, buffer,
    ((TCP_PACKET_HEADER*)buffer)->packet_size - sizeof(TCP_PACKET_HEADER));

  std::cout << "Group NTF arrived." << std::endl;
  auto group_ntf = GetGroupNtf(buffer);
  std::cout << "NTF : " << group_ntf->ntf() << std::endl;
  ntf_id = group_ntf->ntf_id();
  

  /* 4. send join_ac req */
  builder.Clear();
  auto placeholder = builder.CreateString("");
  auto group_req = CreateGroupReq(builder, G_REQ_JOIN_AC, ntf_id, placeholder);
  builder.Finish(group_req);

  header.packet_size = sizeof(TCP_PACKET_HEADER) + builder.GetSize();
  header.packet_id = PACKET_GROUP_REQ;

  write_(socket, (char*)&header, sizeof(TCP_PACKET_HEADER));
  write_(socket, (char*)builder.GetBufferPointer(), builder.GetSize());

  return 0;
}
