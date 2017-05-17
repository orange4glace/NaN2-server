// world.h
#ifndef WORLD_H
#define WORLD_H

#include "../global.h"
#include "world_map.h"
#include "../entity/player.h"
#include "../entity/character.h"

#include "../logger/logger.h"

#include <vector>
#include <queue>
#include <set>
#include <map>
#include <ratio>
#include <ctime>
#include <chrono>

namespace nan2 {

  struct updatable_comparator {
    inline bool operator()(const Updatable* lhs, const Updatable* rhs) const {
        if (lhs->update_order() == rhs->update_order()) return lhs->internal_id() < rhs->internal_id();
        return lhs->update_order() < rhs->update_order();
    }
  };

  class World {

  private:

    std::chrono::high_resolution_clock::time_point last_system_time_;

    int snapshot_send_timer_;
    int last_snapshot_sent_time_;

    // Static World Map data
    WorldMap *world_map_;
    std::map<int, Player*> players_;

    // Update list
    std::set<Updatable*, updatable_comparator> updatable_ready_stage_;
    std::set<Updatable*, updatable_comparator> updatable_set_;
    std::set<Updatable*, updatable_comparator> destroyable_set_;

    void StagingUpdatables();
    void DestroyUpdatables();

    void TakeSnapshot();

    std::queue< std::vector<uint8_t> > send_packet_queue_;

  public:

    World();
    World(WorldMap* world_map);
    ~World();

    const WorldMap* world_map() const;

    void Update();
    void FixedUpdate();

    // Add updatable object to updatable set
    // Object having smaller update order will be updated first
    // Return true if added
    bool AddUpdatable(Updatable* obj);

    // Add updatable object to destroyable set
    // The object will be destroied and freed on update
    // Return number of removed objects
    void DestroyUpdatable(Updatable* obj);

    Player* AddPlayer(int id);
    Player* GetPlayer(int id);
    std::map<int, Player*>& GetPlayers();

    void OnPacketReceived(uint8_t*& buffer, unsigned int& size);
    void ParsePlayerInputPacket(uint8_t* buffer, unsigned int size);

    unsigned int SendPacketQueueSize() const;
    const std::vector<uint8_t> PopSendPacket();

    int last_snapshot_sent_time() const;

  };
  
}

#endif