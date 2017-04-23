// world.h
#ifndef WORLD_H
#define WORLD_H

#include "../global.h"
#include "world_map.h"
#include "../entity/player.h"
#include "../entity/character.h"

#include <vector>
#include <set>
#include <map>

namespace nan2 {

  struct updatableComp {
    inline bool operator()(const Updatable* lhs, const Updatable* rhs) const {
        return lhs->update_order() < rhs->update_order();
    }
  };

  class World {

  private:

    // Static World Map data
    WorldMap *world_map_;
    std::map<int, Player*> players_;

    // Update list
    std::set<Updatable*> updatable_ready_stage_;
    std::set<Updatable*> updatable_set_;
    std::set<Updatable*> destroyable_set_;

    void StagingUpdatables();
    void DestroyUpdatables();

  public:

    World();
    World(WorldMap* world_map);
    ~World();

    const WorldMap& world_map() const;

    void Update();

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

  };
  
}

#endif