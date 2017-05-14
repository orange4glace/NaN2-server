// world_map.cpp
#include "world/world_map.h"

#include "logger/logger.h"

namespace nan2 {

  WorldMap::WorldMap() {
    L_DEBUG << "World map initialized";
  }

  const std::vector<AABB>& WorldMap::GetStaticAABBTileColliders() const {
    return aabb_list_;
  }
  
}