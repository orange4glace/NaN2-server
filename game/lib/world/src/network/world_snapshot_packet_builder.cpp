// packet_parser.cpp
#include "network/world_snapshot_packet_builder.h"

#include "logger/logger.h"

namespace nan2 {

  WorldSnapshotPacketBuilder::WorldSnapshotPacketBuilder() :
    PacketBuilder(1024) {
    
  }

  void WorldSnapshotPacketBuilder::Build(World& world) {
    if (!clean_) Clear();
    clean_ = false;

    std::vector<flatbuffers::Offset<fb::Player>> players_vector;
    auto playerMap = world.GetPlayers();

    for (auto it = playerMap.begin(); it != playerMap.end(); it++) {
      Player& player = *it->second;

      Character& character = player.character();
      CharacterSnapshot& snapshot = character.snapshot();

      std::vector<flatbuffers::Offset<fb::Bullet>> bullets_vector;
      for (const BulletPacket& bullet_packet : snapshot.bullet_packets()) {
        fb::BulletBuilder bullet_builder(builder_);
        bullet_builder.add_time(bullet_packet.time());
        bullet_builder.add_type(bullet_packet.type());
        bullet_builder.add_dir(bullet_packet.dir());
        auto bullet_offset = bullet_builder.Finish();
        bullets_vector.push_back(bullet_offset);
      }
      auto bullets = builder_.CreateVector(bullets_vector);

      fb::Vec2 pos(snapshot.position().x(), snapshot.position().y());
      fb::CharacterBuilder character_builder(builder_);
      character_builder.add_pos(&pos);
      character_builder.add_hp(character.hp());
      character_builder.add_bullets(bullets);
      character_builder.add_dashing_time(0);
      character_builder.add_weapon_magazine(snapshot.weapon_magazine());
      character_builder.add_weapon_ammo(snapshot.weapon_ammo());
      character_builder.add_weapon_cooldown(snapshot.weapon_cooldown());
      character_builder.add_weapon_reload_time(snapshot.weapon_reload_time());
      auto character_offset = character_builder.Finish();

      fb::PlayerBuilder player_builder(builder_);
      player_builder.add_id(player.id());
      player_builder.add_last_input_acked_packet(player.character().last_input_acked_packet_);
      player_builder.add_last_input_remaining_time(player.character().last_input_remaining_time_);
      player_builder.add_character(character_offset);
      auto player_offset = player_builder.Finish();
      players_vector.push_back(player_offset);

      snapshot.Clear();
    }

    auto players = builder_.CreateVector(players_vector);
    fb::WorldBuilder world_builder(builder_);
    world_builder.add_players(players);
    auto world_offset = world_builder.Finish();

    builder_.Finish(world_offset);

    BuildBufferVector();
  }

}