// world.h
#ifndef WORLD_H
#define WORLD_H

#include "../global.h"
#include "world_map.h"
#include "../entity/player.h"
#include "../entity/character.h"
#include "../entity/dropped_item.h"

#include "../network/world_guaranteed_packet_builder.h"
#include "../network/out_packet.h"

#include "../logger/logger.h"

#include <vector>
#include <queue>
#include <set>
#include <map>
#include <functional>
#include <ratio>
#include <ctime>
#include <chrono>

#include <boost/shared_ptr.hpp>

namespace nan2 {

  struct updatable_comparator {
    inline bool operator()(const Updatable* lhs, const Updatable* rhs) const {
        if (lhs->update_order() == rhs->update_order()) return lhs->internal_id() < rhs->internal_id();
        return lhs->update_order() < rhs->update_order();
    }
  };

  struct entity_comparator {
    inline bool operator()(const Entity* lhs, const Entity* rhs) const {
        if (lhs->group() == rhs->group())
            if (lhs->type() == rhs->type())
                return lhs->internal_id() < rhs->internal_id();
            else
                return lhs->type() < rhs->type();
        else
            return lhs->group() < rhs->group();
    }
  };

  class World {

  private:

    std::chrono::high_resolution_clock::time_point last_system_time_;

    int snapshot_send_timer_;
    int last_snapshot_sent_time_;
    int ping_send_timer_;
    int ping_seq_;

    // Static World Map data
    WorldMap *world_map_;

    std::map<int, Player*> players_;
    std::map<entity_group, std::set<Entity*, entity_comparator>> entities_;

    // Update list
    std::set<Updatable*, updatable_comparator> updatable_ready_stage_;
    std::set<Updatable*, updatable_comparator> updatable_set_;
    std::set<Updatable*, updatable_comparator> destroyable_set_;

    std::queue<short> entity_id_pool_;

    WorldGuaranteedPacketBuilder world_guaranteed_packet_builder_;
    std::queue<OutPacket> send_packet_queue_;

    void StagingUpdatables();
    void DestroyUpdatables();

    void TakeSnapshot();

    // Acquire usable entity id
    // Returns 0 if there's no usable id (= entity_id_pool_ is empty)
    short AcquireEntityId();

    // Release entity id and makes it reusable.
    void ReleaseEntityId(short id);

  public:

    /*** For Dev ***/
    std::map<uint64_t, int> cpmap_;
    /*** For Dev ***/

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
    std::set<Entity*, entity_comparator>& entities();

    bool CreateEntity(Entity* entity);
    void DestroyEntity(Entity* entity);

    void IterateEntityGroup(entity_group group, std::function<bool(Entity*)> func);

    bool CreateRandomDroppedItemAt(const Vector2& position, DroppedItem*& spawned_item);

    void OnPacketReceived(boost::shared_ptr<std::vector<int8_t>> buffer, unsigned int& size, uint64_t client_id);
    void ParsePlayerInputPacket(int8_t* buffer, unsigned int size, uint64_t client_id);
    void ParsePongPacket(int8_t* buffer, unsigned int size);

    void SendPingPacket();

    unsigned int SendPacketQueueSize() const;
    const OutPacket PopSendPacket();

    int last_snapshot_sent_time() const;

  };
  
}

#endif
