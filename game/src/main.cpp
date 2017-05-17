#include "header.hpp"

#define MAX_SESSION_CAPACITY 500
#define    MAX_GAME_CAPACITY 500

using namespace nan2;

cpp_redis::redis_client redis_client;

mgne::udp::Server* server;
GameMap game_map;

void packet_handler(mgne::Packet &p)
{
  std::cout << "receiving packet!!" << std::endl;
}

int admit_handler(mgne::Packet &p)
{
  std::cout << "receiving admit req" << std::endl;
  if (p.GetPacketId() != PACKET_ADMIT_REQ) return -1;

  int game_num = -1;
  GameMode game_mode;

  std::string token(p.GetPacketData()->data(), p.GetPacketSize());
  redis_client.hget(token, "game_num", [&game_num](cpp_redis::reply& reply) {
    game_num = std::stoi(reply.as_string());
  });
  redis_client.hget(token, "game_mode", [&game_mode](cpp_redis::reply& reply) {
    game_mode = (GameMode)std::stoi(reply.as_string());
  });

  redis_client.sync_commit();
  
  game_map.Lock();
  auto it = game_map.Find(game_num);
  if (it == game_map.end()) {
    if (game_mode == GameMode::DEATH) {
      it->second.reset(new Game<GameMode::DEATH>(server));
    } else {
      // deal with other game modes..
    }
  }
  it->second->EnterGame(p.GetSessionId());
  game_map.Unlock();

  std::cout << "token : " << token << ", game_num : " << game_num << std::endl;
  return game_num;
}

int main( )
{
  redis_client.connect(REDIS_URL, REDIS_PORT, [](cpp_redis::redis_client&) {
    std::cout << "# ERR: redis connection lost" << std::endl;
    exit(-1);
  });

  redis_client.sync_commit();
  std::cout << " - Redis Connection success" << std::endl;

  boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::udp::v4(), 4000);
  server = new mgne::udp::Server(endpoint, MAX_SESSION_CAPACITY, 3, 3,
    packet_handler, admit_handler);

  server->Run();
  return 0;
}
