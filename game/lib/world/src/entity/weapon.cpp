// weapon.cpp
#include "entity/weapon.h"

#include "entity/character.h"
#include "math_helper.h"

#include "logger/logger.h"

namespace nan2 {

  Weapon::Weapon(World* world, entity_type type,
    int max_cooldown, int max_reload_time, int max_ammo, int max_magazine,
    const Vector2& bullet_point) :
    Updatable(world, Entity::GROUP_WEAPON, type),
    character_(nullptr),
    max_cooldown_(max_cooldown),
    max_reload_time_(max_reload_time),
    max_ammo_(max_ammo),
    max_magazine_(max_magazine),
    cooldown_(0),
    reload_time_(0),
    ammo_(max_ammo),
    magazine_(max_magazine - max_ammo),
    is_reloading_(false),
    bullet_point_(bullet_point) {
      update_order_ = 0;
  }

  bool Weapon::CanFire() {
    return
      character_ != nullptr && 
      cooldown_ <= 0 &&
      reload_time_ <= 0 &&
      ammo_ > 0;
  }

  void Weapon::AfterFired() {
    ammo_ -= 1;
    if (ammo_ == 0) {
      reload_time_ = max_reload_time_;
      ammo_ = (magazine_ < max_ammo_ ? magazine_ : max_ammo_);
      magazine_ -= ammo_;
    }
    else {
      cooldown_ = max_cooldown_;
    }
  }

  void Weapon::Update() {
    DecreaseByDT(cooldown_);
    DecreaseByDT(reload_time_);
  }

  void Weapon::SetCharacter(Character* character) {
    character_= character;
  }

  Vector2 Weapon::GetBulletPoint(unsigned char dir, const Vector2& position) const {
    unsigned char hdir = (unsigned char)((dir + (252 / 4) + 252) % 252);
    bool view_right = (hdir <= (252 / 2));
    MathHelper::instance().normal_dir_252(dir);
    Vector2 arm_anchor = position + Vector2(3.0f * (view_right ? -1 : 1), -2);
    float rad = MathHelper::instance().DegToRad(MathHelper::instance().normal_angle_252(dir));
    Vector2 arm_vector = MathHelper::instance().RotateVector2(Vector2(5, -0.5f), rad);
    Vector2 weapon_attacher_anchor = arm_anchor + arm_vector;
    Vector2 bullet_point = MathHelper::instance().RotateVector2(bullet_point_, rad) + weapon_attacher_anchor;
    L_DEBUG << position << " " << bullet_point << " " << weapon_attacher_anchor;
    L_DEBUG << arm_anchor << " " << arm_vector;
    return bullet_point;
  }

  int Weapon::magazine() {
    return magazine_;
  }

  int Weapon::ammo() {
    return ammo_;
  }

  int Weapon::cooldown() {
    return cooldown_;
  }
  
  int Weapon::reload_time() {
    return reload_time_;
  }

}