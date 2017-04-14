// world_map.cpp
#include "world/world_map.h"

namespace nan2 {

  WorldMap::WorldMap() {
  }

  const std::vector<AABB>& WorldMap::GetStaticAABBTileColliders() const {
    return aabb_list_;
  }
  
}