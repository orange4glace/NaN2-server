//
// game.hpp
//

#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <vector>

#include <mgne/server.hpp>
#include <mgne/pattern/thread_safe.hpp>

#include "../../include/game_info.hpp"
#include "../lib/world/include/world/world.h"

namespace nan2 {
class InterfaceGame {
public:
  virtual int EnterGame(int client_id) = 0;

  virtual void SendToClient(const std::vector<char>& message, int client_id) = 0;
  virtual void SendToAll(const std::vector<char>& message) = 0;

  virtual World& GetWorld() = 0;
  virtual boost::asio::deadline_timer& GetTimer() = 0;
  virtual int GetId() = 0;

  virtual ~InterfaceGame() = default;
};

template<GameMode mode>
class Game : public InterfaceGame {
public:
  // TODO : complete this class...

  Game(mgne::udp::Server* server, boost::asio::io_service& update_service,
    int num)
    : server_(server)
    , update_timer_(update_service)
    , id_(num)
  {
    num_people_ = 0;
  }

  int EnterGame(int client_id)
  {
    if (num_people_ == sizes[mode]) return -1;
    clients_[num_people_++] = client_id;
    if (num_people_ == sizes[mode]) return 0;
    return 1;
  }

  void SendToAll(const std::vector<char>& message)
  {
    mgne::Packet tmp(message.size(), 0, message.data(),
      mgne::Packet::PacketType::PACKET_UDP);
    for (int client : clients_) {
      server_->GetSessionManager().Send(client, tmp);
    }
  }

  void SendToClient(const std::vector<char>& message, int client)
  {
    mgne::Packet tmp(message.size(), 0, message.data(),
      mgne::Packet::PacketType::PACKET_UDP);
    server_->GetSessionManager().Send(client, tmp);
  }

  World& GetWorld() { return world_; }
  boost::asio::deadline_timer& GetTimer() { return update_timer_; }
  int GetId() { return id_; }

private:
  mgne::udp::Server* server_;
  boost::asio::deadline_timer update_timer_;
  World world_;

  std::array<int, sizes[mode]> clients_;
  int num_people_;
  int id_;
};

class GameMap : mgne::pattern::ThreadSafe {
private:
  typedef std::shared_ptr<InterfaceGame> game_ptr;
  typedef std::unordered_map<int, game_ptr> map; 

public: 
  game_ptr& operator[](int key) { return map_[key]; }

  void Lock() { ThreadSafe::Lock(); }
  void Unlock() { ThreadSafe::Unlock(); }

  map::iterator Find(int key) { return map_.find(key); }
  map::iterator begin() { return map_.begin(); }
  map::iterator end() { return map_.end(); }

  // how about Erase?

private:
  std::unordered_map<int, game_ptr> map_;
};
}

#endif
