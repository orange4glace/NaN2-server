#include <iostream>
#include <random>

#include <mgne/protocol.hpp>
#include <boost/asio.hpp>

#include "../include/signup_req_generated.h"
#include "../include/signup_ans_generated.h"
#include "../include/protocol.hpp"

using namespace NaN2::Login;

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

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribution(10000,99999);
  TCP_PACKET_HEADER header;
  flatbuffers::FlatBufferBuilder builder(1024);

  std::string uuid_str = "550e8400-e29b-41d4-a716-4466554" +
    std::to_string(distribution(gen));
  auto uuid = builder.CreateString(uuid_str);
  auto user_name = builder.CreateString("test");
  auto signup_req = CreateSignupReq(builder, uuid, user_name);
  builder.Finish(signup_req);

  header.packet_size = sizeof(TCP_PACKET_HEADER) + builder.GetSize();
  header.packet_id = PACKET_SIGNUP_REQ;

  socket.write_some(boost::asio::buffer((char*)&header, 
    sizeof(TCP_PACKET_HEADER)));
  socket.write_some(boost::asio::buffer((char*)builder.GetBufferPointer(), 
    builder.GetSize()));
  std::cout << "Sended : " << builder.GetSize() << std::endl;

  char buffer[1024];
  boost::asio::read(socket, boost::asio::buffer(buffer,
    sizeof(TCP_PACKET_HEADER)));

  short packet_id = ((TCP_PACKET_HEADER*)buffer)->packet_id;
  short packet_size = ((TCP_PACKET_HEADER*)buffer)->packet_size;

  boost::asio::read(socket, boost::asio::buffer(buffer,
    packet_size - sizeof(TCP_PACKET_HEADER)));

  std::cout << "Received : " << packet_size << std::endl;

  auto signup_ans = GetSignupAns(buffer);
  auto ans = signup_ans->ans();
  std::string user_tag = signup_ans->user_tag()->str();

  std::cout << "Answer : " << ans << std::endl;
  std::cout << "User tag : " << user_tag << std::endl;

  return 0;
}
