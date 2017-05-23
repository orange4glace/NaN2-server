#include "header.hpp"

#define MAX_SESSION_CAPACITY 500
#define    MAX_GAME_CAPACITY 500 
#define WORLD_THREAD         3

using namespace nan2;

typedef mgne::pattern::ThreadJobQueue<std::function<void()>> WorldQueue;

cpp_redis::redis_client redis_client;

boost::asio::io_service update_service;
boost::asio::io_service::work update_work(update_service);
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

  std::shared_ptr<std::vector<char>> ptr = p.GetPacketData();
  std::function<void()> process_packet([&, ptr](){
    World& world = games[session_id]->GetWorld();
    unsigned int packet_size = p.GetPacketSize();
    world.OnPacketReceived(ptr, packet_size, p.GetSessionId());
  });
  world_queues[game->GetId() % WORLD_THREAD].Push(process_packet);
}

int admit_handler(mgne::Packet &p)
{
  if (p.GetPacketId() != PACKET_ADMIT_REQ) return -1;

  int game_num = -1;
  GameMode game_mode;

  std::string token(p.GetPacketData()->data(), p.GetPacketSize());
  redis_client.hget(token, "game_num", [&game_num](cpp_redis::reply& reply) {
    game_num = std::stoi(reply.as_string());
  });
  redis_client.sync_commit();
  
  game_map.Lock();
  if (game_map.Find(game_num) == game_map.end()) {
    redis_client.get(std::to_string(game_num),
      [&game_mode](cpp_redis::reply& reply) {
      game_mode = (GameMode)std::stoi(reply.as_string());
    });
    redis_client.sync_commit();
    std::cout << "creating new game..!" << std::endl;
    std::cout << "game num : " << game_num << ", game mode: " << game_mode <<
      std::endl;
    if (game_mode == GameMode::DEATH) {
      game_map[game_num].reset(new Game<GameMode::DEATH>(server, update_service,
        game_num));
    } else {
      // deal with other game modes..
    }
  }
  games[p.GetSessionId()] = game_map[game_num].get();
  std::cout << games[p.GetSessionId()]->GetId() << std::endl;
  if (games[p.GetSessionId()]->EnterGame(p.GetSessionId()) == 0) {
    std::cout << "All entered..! start updating.." << std::endl;
    std::cout << games[p.GetSessionId()]->GetId() << std::endl;
    update_world(games[p.GetSessionId()]);
  }
  // TODO : need to reply
  game_map.Unlock();
  return game_num;
}

int main( )
{
  try {
    redis_client.connect(REDIS_URL, REDIS_PORT, [](cpp_redis::redis_client&) {
      std::cout << "# ERR: redis connection lost" << std::endl;
      exit(-1);
    });
    std::cout << " - Redis Connection success" << std::endl;
  } catch(...) {
    std::cout << "# ERR: redis connection lost" << std::endl;
  }

  boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::udp::v4(), 4000);
  server = new mgne::udp::Server(endpoint, MAX_SESSION_CAPACITY, 3, 3,
    packet_handler, admit_handler);

  server->RunNonBlock();

  boost::thread_group tg;
  for (int i = 0; i < WORLD_THREAD; i++) {
    tg.create_thread([&,i](){
      while(1) {
        std::function<void()> job;
        if (world_queues[i].Pop(job)) {
          job();
        }
      }
    });
  }

  update_service.run();
  tg.join_all();

  return 0;
}

