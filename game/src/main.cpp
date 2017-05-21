#include "header.hpp"

#define MAX_SESSION_CAPACITY 500
#define    MAX_GAME_CAPACITY 500

#define WORLD_THREAD         3

using namespace nan2;

typedef mgne::pattern::ThreadJobQueue<std::function<void()>> WorldQueue;

cpp_redis::redis_client redis_client;

boost::asio::io_service update_service;
std::array<WorldQueue, WORLD_THREAD> world_queues;
std::array<InterfaceGame*, MAX_SESSION_CAPACITY> games;

mgne::udp::Server* server;
GameMap game_map;

void update_and_send(InterfaceGame* game)
{
  World& world = game->GetWorld();
  world.Update();
  while (world.SendPacketQueueSize()) {
    OutPacket out_packet = world.PopSendPacket();
    if (out_packet.type() == OutPacket::BROADCAST) {
      game->SendToAll(out_packet.data());
    } else {
      game->SendToClient(out_packet.data(), out_packet.receiver());
    }
  }
}

void update_world(InterfaceGame* game)
{
  std::function<void()> world_process(std::bind(update_and_send, game));
  world_queues[game->GetId() % WORLD_THREAD].Push(world_process);
  game->GetTimer().expires_from_now(boost::posix_time::milliseconds(16));
  // need to be calculated
  game->GetTimer().async_wait(boost::bind(update_world, game));
}

void packet_handler(mgne::Packet &p)
{
  int session_id = p.GetSessionId();
  InterfaceGame* game = games[session_id];

  std::function<void()> process_packet([&](){
    World& world = games[session_id]->GetWorld();
    std::shared_ptr<std::vector<char>> ptr = p.GetPacketData();
    //world.OnPacketReceived(ptr, p.GetPacketSize(), p.GetSessionId());
  });
  world_queues[game->GetId() % WORLD_THREAD].Push(process_packet);
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
      it->second.reset(new Game<GameMode::DEATH>(server, update_service,
        game_num));
    } else {
      // deal with other game modes..
    }
  }

  games[p.GetSessionId()] = it->second.get();
  if (it->second->EnterGame(p.GetSessionId()) == 0) {
    update_world(it->second.get());
  }

  // TODO : need to reply
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

  server->RunNonBlock();

  for (int i = 0; i < WORLD_THREAD; i++) {
    std::thread{[&,=i](){
      while (1) {
        auto job = world_queues[i].Pop();
        job();
      }
    }};
  }
  return 0;
}
