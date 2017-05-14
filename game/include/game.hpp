//
// game.hpp
//

#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <vector>

#include <mgne/server.hpp>
#include <mgne/pattern/thread_safe.hpp>

#include "../../include/game_info.hpp"

namespace nan2 {
class InterfaceGame {
public:
  virtual bool EnterGame(int client_id) = 0;

  virtual void SendToClient(char* message, int client_id) = 0;
  virtual void SendToAllExcept(char* message, int client_id) = 0;
  virtual void SendToAll(char* message) = 0;

  virtual ~InterfaceGame() = default;
};

template<GameMode mode>
class Game : public InterfaceGame {
public:
  // TODO : complete this class...

  Game(mgne::udp::Server* server) 
    : server_(server)
  {
  }

  bool EnterGame(int client_id)
  {
    return true;
  }

  void SendToClient(char* message, int client_id)
  {
  }

  void SendToAllExcept(char* message, int client_id)
  {
  }

  void SendToAll(char* message)
  {
  }

private:
  std::array<int, sizes[mode]> clients_;
  mgne::udp::Server* server_;
};

class GameMap : mgne::pattern::ThreadSafe {
private:
  typedef std::unique_ptr<InterfaceGame> game_ptr;
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
  std::unordered_map<int, std::unique_ptr<InterfaceGame>> map_;
};
}

#endif
