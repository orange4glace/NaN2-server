#include <iostream>
#include <string>

#include <mgne/protocol.hpp>
#include <boost/asio.hpp>

#include "../include/join_req_generated.h"
#include "../include/join_ans_generated.h"
#include "../include/protocol.hpp"

using namespace nan2::lobby;

const char SERVER_IP[] = "127.0.0.1";
const unsigned short PORT_NUMBER = 4000;

int main()
{
  boost::asio::io_service io_service;
  boost::asio::ip::tcp::endpoint endpoint(
    boost::asio::ip::address::from_string(SERVER_IP), PORT_NUMBER);
  boost::system::error_code connect_error;
  boost::asio::ip::tcp::socket socket(io_service);

  socket.connect(endpoint, connect_error);

  TCP_PACKET_HEADER header;
  flatbuffers::FlatBufferBuilder builder(1024);

  std::string token_str = "iX2ocLIfUZ9nepY5CUjd";
  auto token = builder.CreateString(token_str);
  auto join_req = CreateJoinReq(builder, token);
  builder.Finish(join_req);

  header.packet_size = sizeof(TCP_PACKET_HEADER) + builder.GetSize();
  header.packet_id = PACKET_JOIN_REQ;

  socket.write_some(boost::asio::buffer((char*)&header,
    sizeof(TCP_PACKET_HEADER)));
  socket.write_some(boost::asio::buffer((char*)builder.GetBufferPointer(),
    builder.GetSize()));
  std::cout << "Sended : " << builder.GetSize() << std::endl;

  return 0;
}
