// machine_gun_bullet.cpp
#include "entity/machine_gun_bullet.h"

#include "entity/character.h"
#include "world/world.h"
#include "time.h"

#include <nan2/math/aabb.h>

#include <vector>
#include <math.h>

namespace nan2 {

  MachineGunBullet::MachineGunBullet(World* world, const Vector2& position, const Vector2& angle, const int collision_mask) :
    Bullet(world, collision_mask),
    position_(position),
    angle_(angle),
    SPEED_(200.0f) {
  }

  const AABB MachineGunBullet::collider() const {

  }

  void MachineGunBullet::Update() {
    auto players = world_->GetPlayers();
    AABB thisCollider = collider();
    Vector2 dv = angle_ * SPEED_;
    
    const void* rCollider = nullptr;

    const Character* rCharacter = nullptr;
    float rf = INFINITY;
    for (auto player : players) {
      Character& character = player.second->character();
      // Get interpolated target data relative to bullet initiator
      CharacterTickData td = character.GetInterpolatedDataAt(Time::current_time() - interpolation_time_);
      const AABB& collider = td.collider();
      float cf = AABB::SweptAABB(thisCollider, dv, collider, Vector2::ZERO);
      if (cf < rf) {
        rCharacter = &character;
        rCollider = &character;
        rf = cf;
      }
    }

    const AABB* rTile = nullptr;
    const std::vector<AABB>& world_map = world_->world_map()->GetStaticAABBTileColliders();
    for (const AABB& tile : world_map) {
      float cf = AABB::SweptAABB(thisCollider, dv, tile, Vector2::ZERO);
      if (cf < rf) {
        rTile = &tile;
        rCollider = &tile;
        rf = cf;
      }
    }

    if (rCollider != nullptr) {
      if (rCollider == rCharacter) {

      }
      else if (rCollider == rTile) {

      }
      Destroy();
    }
    else {
      position_ += dv;
    }
  }
  
}