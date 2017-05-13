#include "header.hpp"

#define MAX_CAPACITY 500

using namespace nan2;

cpp_redis::redis_client redis_client;

mgne::udp::Server* server;

void packet_handler(mgne::Packet &p)
{
  std::cout << "receiving packet!!" << std::endl;
}

int admit_handler(mgne::Packet &p)
{
  std::cout << "receiving admit req" << std::endl;
  if (p.GetPacketId() != PACKET_ADMIT_REQ) return -1;

  int game_num = -1;

  std::string token(p.GetPacketData()->data(), p.GetPacketSize());
  redis_client.hget(token, "game_num", [&game_num](cpp_redis::reply& reply) {
    game_num = std::stoi(reply.as_string());
  });

  redis_client.sync_commit();

  std::cout << "token : " << token << ", game_num : " << game_num << std::endl;
  return game_num;
}

int main()
{
  redis_client.connect(REDIS_URL, REDIS_PORT, [](cpp_redis::redis_client&) {
    std::cout << "# ERR: redis connection lost" << std::endl;
    exit(-1);
  });

  redis_client.sync_commit();
  std::cout << " - Redis Connection success" << std::endl;

  boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::udp::v4(), 4000);
  server = new mgne::udp::Server(endpoint, MAX_CAPACITY, 3, 3, packet_handler,
    admit_handler);

  server->Run();
  return 0;
}
