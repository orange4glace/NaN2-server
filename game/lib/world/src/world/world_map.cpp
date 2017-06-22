// world_map.cpp
#include "world/world_map.h"

#include "logger/logger.h"

namespace nan2 {

  WorldMap::WorldMap() {
    L_DEBUG << "World map initialized";
    aabb_list_.push_back(AABB(Rect(1136, 1120, 576, 32)));
    aabb_list_.push_back(AABB(Rect(1136, 496, 576, 32)));
    aabb_list_.push_back(AABB(Rect(832, 832, 32, 608)));
    aabb_list_.push_back(AABB(Rect(1442, 832, 32, 608)));
    aabb_list_.push_back(AABB(Rect(1328, 872, 192, 48)));
    aabb_list_.push_back(AABB(Rect(1040, 872, 192, 48)));
    aabb_list_.push_back(AABB(Rect(976, 944, 64, 128)));
    aabb_list_.push_back(AABB(Rect(1136, 1120, 576, 32)));
    aabb_list_.push_back(AABB(Rect(1344, 1000, 32, 48)));
  }

  const std::vector<AABB>& WorldMap::GetStaticAABBTileColliders() const {
    return aabb_list_;
  }
  
}