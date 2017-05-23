#include <iostream>
#include <string>

#include <mgne/protocol.hpp>
#include <boost/asio.hpp>

const char SERVER_IP[] = "127.0.0.1";
const unsigned short PORT_NUMBER = 4000;

boost::asio::ip::udp::endpoint endpoint(
  boost::asio::ip::address::from_string(SERVER_IP), PORT_NUMBER);

void write_(boost::asio::ip::udp::socket& socket, const char* buffer, int len)
{
  std::cout << buffer << std::endl;
  socket.send_to(boost::asio::buffer(buffer, len), endpoint);
}

int read_(boost::asio::ip::udp::socket& socket, char* buffer, int len)
{
  return socket.receive_from(boost::asio::buffer(buffer, len), endpoint);
}

int main(int argc, char *argv[])
{
  boost::asio::io_service io_service;
  boost::asio::ip::udp::endpoint ep(boost::asio::ip::udp::v4(), 0);
  boost::asio::ip::udp::socket socket(io_service, ep);

  int len;
  char buffer[1024];

  std::string token(argv[0]);

  ((UDP_PACKET_HEADER*)buffer)->packet_size = sizeof(UDP_PACKET_HEADER) + token.size();
  ((UDP_PACKET_HEADER*)buffer)->packet_id = PACKET_ADMIT_REQ;

  for (int i = 0; i < 20; i++) {
    buffer[i + sizeof(UDP_PACKET_HEADER)] = token[i];
  }

  write_(socket, buffer, sizeof(UDP_PACKET_HEADER) + token.size());

  read_(socket, buffer, sizeof(UDP_PACKET_HEADER) + sizeof(short));
  std::cout << *((short*)(buffer + sizeof(UDP_PACKET_HEADER))) << std::endl;

  return 0;
}
