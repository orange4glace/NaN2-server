#include "header.hpp"

using namespace nan2;

void packet_handler(mgne::Packet &p)
{
  std::cout << "receiving packet!!" << std::endl;
}

int admit_handler(mgne::Packet &p)
{
  std::cout << "receiving admit req" << std::endl;
  return 1;
}

int main()
{
  boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::udp::v4(), 4000);
  mgne::udp::Server server(endpoint, 500, 3, 3, packet_handler, admit_handler);
  return 0;
}
