// machine_gun_bullet.cpp
#include "entity/machine_gun_bullet.h"

#include "entity/character.h"
#include "world/world.h"
#include "time.h"

#include <nan2/math/rect.h>
#include <nan2/math/aabb.h>

#include <vector>
#include <math.h>

namespace nan2 {

  MachineGunBullet::MachineGunBullet(World* world, const Vector2& position, const Vector2& angle, int player_id, int collision_mask) :
    Bullet(world, player_id, collision_mask),
    angle_(angle),
    SPEED_(250.0f) {
      position_ = position;
      damage_ = 2;
  }

  const AABB MachineGunBullet::collider() const {
    return AABB(Rect(position_.x(), position_.y(), 3.0f, 3.0f));
  }

  void MachineGunBullet::Update() {
  }

  void MachineGunBullet::FixedUpdate() {
    Bullet::Update();

    auto players = world_->GetPlayers();
    AABB thisCollider = collider();
    Vector2 dv = angle_ * SPEED_ * (Time::fixed_delta_time() / 1000.0f);
    
    const void* rCollider = nullptr;
    Character* rCharacter = nullptr;
    RootBox* rRootBox = nullptr;

    float rf = INFINITY;
    for (auto player : players) {
      Character& character = player.second->character();
      // Check layer mask
      //if (((1 << character.layer()) & collision_mask_) == 0) continue;
      if (player.second->id() == player_id_) continue;
      // Get interpolated target data relative to bullet initiator
      CharacterTickData td = character.GetInterpolatedDataAt(Time::current_fixed_time() - interpolation_time_);
      const AABB& collider = td.collider();
      bool collided;
      float cf = AABB::SweptAABB(thisCollider, dv, collider, Vector2::ZERO, collided);
      if (!collided) continue;
      if (cf < rf) {
        rCharacter = &character;
        rCollider = &character;
        rf = cf;
      }
    }

    auto root_boxes = world_->root_boxes();

    for (auto root_box : root_boxes) {
      bool collided;
      const AABB& collider = root_box->collider();
      float cf = AABB::SweptAABB(thisCollider, dv, collider, Vector2::ZERO, collided);
      if (!collided) continue;
      if (cf < rf) {
        rRootBox = root_box;
        rCollider = root_box;
        rf = cf;
      }
    }

    const AABB* rTile = nullptr;
    const std::vector<AABB>& world_map = world_->world_map()->GetStaticAABBTileColliders();
    for (const AABB& tile : world_map) {
      bool collided;
      float cf = AABB::SweptAABB(thisCollider, dv, tile, Vector2::ZERO, collided);
      if (!collided) continue;
      if (cf < rf) {
        rTile = &tile;
        rCollider = &tile;
        rf = cf;
      }
    }

    if (rCollider != nullptr) {
      if (rCollider == rCharacter) {
        L_DEBUG << "#### Collision detected " << rCharacter->player().id();
        if (!rCharacter->is_dashing()) {
          rCharacter->AddHP(-damage_);
          Destroy();
        }
      }
      else if (rCollider == rRootBox) {
        L_DEBUG << "#### Collision detected Root box";
        Destroy();
      }
      else if (rCollider == rTile) {
        Destroy();
      }
    }
    else {
      position_ += dv;
    }
  }
  
}