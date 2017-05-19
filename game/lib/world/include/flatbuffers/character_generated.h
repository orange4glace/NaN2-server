// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_CHARACTER_NAN2_GAME_WORLD_H_
#define FLATBUFFERS_GENERATED_CHARACTER_NAN2_GAME_WORLD_H_

#include "flatbuffers/flatbuffers.h"

#include "bullet_generated.h"
#include "vec2_generated.h"

namespace nan2 {
namespace game {
namespace world {

struct Character;

struct Character FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_POS = 4,
    VT_HP = 6,
    VT_BULLETS = 8,
    VT_DASH_TIME = 10,
    VT_DASH_COOLDOWN = 12,
    VT_DASH_DIR = 14,
    VT_WEAPON_MAGAZINE = 16,
    VT_WEAPON_AMMO = 18,
    VT_WEAPON_COOLDOWN = 20,
    VT_WEAPON_RELOAD_TIME = 22
  };
  const nan2::game::world::Vec2 *pos() const {
    return GetStruct<const nan2::game::world::Vec2 *>(VT_POS);
  }
  int32_t hp() const {
    return GetField<int32_t>(VT_HP, 0);
  }
  const flatbuffers::Vector<flatbuffers::Offset<nan2::game::world::Bullet>> *bullets() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<nan2::game::world::Bullet>> *>(VT_BULLETS);
  }
  int32_t dash_time() const {
    return GetField<int32_t>(VT_DASH_TIME, 0);
  }
  int32_t dash_cooldown() const {
    return GetField<int32_t>(VT_DASH_COOLDOWN, 0);
  }
  uint8_t dash_dir() const {
    return GetField<uint8_t>(VT_DASH_DIR, 0);
  }
  int32_t weapon_magazine() const {
    return GetField<int32_t>(VT_WEAPON_MAGAZINE, 0);
  }
  int32_t weapon_ammo() const {
    return GetField<int32_t>(VT_WEAPON_AMMO, 0);
  }
  int32_t weapon_cooldown() const {
    return GetField<int32_t>(VT_WEAPON_COOLDOWN, 0);
  }
  int32_t weapon_reload_time() const {
    return GetField<int32_t>(VT_WEAPON_RELOAD_TIME, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<nan2::game::world::Vec2>(verifier, VT_POS) &&
           VerifyField<int32_t>(verifier, VT_HP) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_BULLETS) &&
           verifier.Verify(bullets()) &&
           verifier.VerifyVectorOfTables(bullets()) &&
           VerifyField<int32_t>(verifier, VT_DASH_TIME) &&
           VerifyField<int32_t>(verifier, VT_DASH_COOLDOWN) &&
           VerifyField<uint8_t>(verifier, VT_DASH_DIR) &&
           VerifyField<int32_t>(verifier, VT_WEAPON_MAGAZINE) &&
           VerifyField<int32_t>(verifier, VT_WEAPON_AMMO) &&
           VerifyField<int32_t>(verifier, VT_WEAPON_COOLDOWN) &&
           VerifyField<int32_t>(verifier, VT_WEAPON_RELOAD_TIME) &&
           verifier.EndTable();
  }
};

struct CharacterBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_pos(const nan2::game::world::Vec2 *pos) {
    fbb_.AddStruct(Character::VT_POS, pos);
  }
  void add_hp(int32_t hp) {
    fbb_.AddElement<int32_t>(Character::VT_HP, hp, 0);
  }
  void add_bullets(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<nan2::game::world::Bullet>>> bullets) {
    fbb_.AddOffset(Character::VT_BULLETS, bullets);
  }
  void add_dash_time(int32_t dash_time) {
    fbb_.AddElement<int32_t>(Character::VT_DASH_TIME, dash_time, 0);
  }
  void add_dash_cooldown(int32_t dash_cooldown) {
    fbb_.AddElement<int32_t>(Character::VT_DASH_COOLDOWN, dash_cooldown, 0);
  }
  void add_dash_dir(uint8_t dash_dir) {
    fbb_.AddElement<uint8_t>(Character::VT_DASH_DIR, dash_dir, 0);
  }
  void add_weapon_magazine(int32_t weapon_magazine) {
    fbb_.AddElement<int32_t>(Character::VT_WEAPON_MAGAZINE, weapon_magazine, 0);
  }
  void add_weapon_ammo(int32_t weapon_ammo) {
    fbb_.AddElement<int32_t>(Character::VT_WEAPON_AMMO, weapon_ammo, 0);
  }
  void add_weapon_cooldown(int32_t weapon_cooldown) {
    fbb_.AddElement<int32_t>(Character::VT_WEAPON_COOLDOWN, weapon_cooldown, 0);
  }
  void add_weapon_reload_time(int32_t weapon_reload_time) {
    fbb_.AddElement<int32_t>(Character::VT_WEAPON_RELOAD_TIME, weapon_reload_time, 0);
  }
  CharacterBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  CharacterBuilder &operator=(const CharacterBuilder &);
  flatbuffers::Offset<Character> Finish() {
    const auto end = fbb_.EndTable(start_, 10);
    auto o = flatbuffers::Offset<Character>(end);
    return o;
  }
};

inline flatbuffers::Offset<Character> CreateCharacter(
    flatbuffers::FlatBufferBuilder &_fbb,
    const nan2::game::world::Vec2 *pos = 0,
    int32_t hp = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<nan2::game::world::Bullet>>> bullets = 0,
    int32_t dash_time = 0,
    int32_t dash_cooldown = 0,
    uint8_t dash_dir = 0,
    int32_t weapon_magazine = 0,
    int32_t weapon_ammo = 0,
    int32_t weapon_cooldown = 0,
    int32_t weapon_reload_time = 0) {
  CharacterBuilder builder_(_fbb);
  builder_.add_weapon_reload_time(weapon_reload_time);
  builder_.add_weapon_cooldown(weapon_cooldown);
  builder_.add_weapon_ammo(weapon_ammo);
  builder_.add_weapon_magazine(weapon_magazine);
  builder_.add_dash_cooldown(dash_cooldown);
  builder_.add_dash_time(dash_time);
  builder_.add_bullets(bullets);
  builder_.add_hp(hp);
  builder_.add_pos(pos);
  builder_.add_dash_dir(dash_dir);
  return builder_.Finish();
}

inline flatbuffers::Offset<Character> CreateCharacterDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const nan2::game::world::Vec2 *pos = 0,
    int32_t hp = 0,
    const std::vector<flatbuffers::Offset<nan2::game::world::Bullet>> *bullets = nullptr,
    int32_t dash_time = 0,
    int32_t dash_cooldown = 0,
    uint8_t dash_dir = 0,
    int32_t weapon_magazine = 0,
    int32_t weapon_ammo = 0,
    int32_t weapon_cooldown = 0,
    int32_t weapon_reload_time = 0) {
  return nan2::game::world::CreateCharacter(
      _fbb,
      pos,
      hp,
      bullets ? _fbb.CreateVector<flatbuffers::Offset<nan2::game::world::Bullet>>(*bullets) : 0,
      dash_time,
      dash_cooldown,
      dash_dir,
      weapon_magazine,
      weapon_ammo,
      weapon_cooldown,
      weapon_reload_time);
}

}  // namespace world
}  // namespace game
}  // namespace nan2

#endif  // FLATBUFFERS_GENERATED_CHARACTER_NAN2_GAME_WORLD_H_
