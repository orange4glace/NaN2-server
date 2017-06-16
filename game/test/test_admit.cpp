#include <iostream>
#include <string>
#include <thread>

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
  unsigned char packet_sample[] = { 0x04, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x08, 0x00, 0x0C, 0x00, 0x04, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x10, 0x00, 0x04, 0x00, 0x0B, 0x00, 0x0A, 0x00, 0x09, 0x00, 0x0C, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x30, 0x11, 0x00, 0x00, 0x00};

  std::cout << sizeof(packet_sample) << std::endl;

  std::string token(argv[1]);

  ((UDP_PACKET_HEADER*)buffer)->packet_size = sizeof(UDP_PACKET_HEADER) + token.size();
  ((UDP_PACKET_HEADER*)buffer)->packet_id = PACKET_ADMIT_REQ;

  for (int i = 0; i < 20; i++) {
    buffer[i + sizeof(UDP_PACKET_HEADER)] = token[i];
  }

  write_(socket, buffer, sizeof(UDP_PACKET_HEADER) + token.size());
  
  ((UDP_PACKET_HEADER*)buffer)->packet_size = sizeof(UDP_PACKET_HEADER) + 72;
  while(1)
  {
    read_(socket, buffer, sizeof(UDP_PACKET_HEADER) + sizeof(short));
  }

  return 0;
}
