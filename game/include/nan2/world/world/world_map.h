// world_map.h
#ifndef WORLD_MAP_H
#define WORLD_MAP_H

#include <nan2/math/aabb.h>

#include <vector>

// WorldMap contains AABB Colliders presenting static maptile

namespace nan2 {

  class WorldMap {

  private:

    // Map identifier
    short id;
    std::vector<AABB> aabb_list_;

  public:

    WorldMap();
    const std::vector<AABB>& GetStaticAABBTileColliders() const;

  };
  
}

#endif