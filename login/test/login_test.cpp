#include <iostream>
#include <random>

#include <mgne/protocol.hpp>
#include <boost/asio.hpp>

#include "../include/login_req_generated.h"
#include "../include/login_ans_generated.h"
#include "../include/protocol.hpp"

using namespace nan2::login;

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

  std::string uuid_str = "550e8400-e29b-41d4-a716-446655446391";
  auto uuid = builder.CreateString(uuid_str);
  auto login_req = CreateLoginReq(builder, uuid);
  builder.Finish(login_req);

  header.packet_size = sizeof(TCP_PACKET_HEADER) + builder.GetSize();
  header.packet_id = PACKET_LOGIN_REQ;

  socket.write_some(boost::asio::buffer((char*)&header,
    sizeof(TCP_PACKET_HEADER)));
  socket.write_some(boost::asio::buffer((char*)builder.GetBufferPointer(),
    builder.GetSize()));
  std::cout << "Sended : " << builder.GetSize() << std::endl;

  return 0;
}
