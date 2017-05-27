// world_snapshot_packet_builder.cpp
#include "network/world_snapshot_packet_builder.h"

#include "logger/logger.h"

namespace nan2 {

  WorldSnapshotPacketBuilder::WorldSnapshotPacketBuilder() :
    PacketBuilder() {
    packet_type_ = PacketType::SNAPSHOT;
  }

  void WorldSnapshotPacketBuilder::Build(World& world) {
    if (!clean_) Clear();
    clean_ = false;

    AppendInt(PacketType::SNAPSHOT);

    flatbuffers::FlatBufferBuilder builder;

    std::vector<flatbuffers::Offset<fb::Player>> players_vector;
    auto playerMap = world.GetPlayers();

    for (auto it = playerMap.begin(); it != playerMap.end(); it++) {
      Player& player = *it->second;

      Character& character = player.character();
      CharacterSnapshot& snapshot = character.snapshot();

      std::vector<flatbuffers::Offset<fb::Bullet>> bullets_vector;
      for (const BulletPacket& bullet_packet : snapshot.bullet_packets()) {
        fb::BulletBuilder bullet_builder(builder);
        bullet_builder.add_time(bullet_packet.time());
        bullet_builder.add_type(bullet_packet.type());
        bullet_builder.add_dir(bullet_packet.dir());
        auto bullet_offset = bullet_builder.Finish();
        bullets_vector.push_back(bullet_offset);
      }
      auto bullets = builder.CreateVector(bullets_vector);

      std::vector<flatbuffers::Offset<fb::EntityObtained>> entities_obtained_vector;
      while (character.net_entities_obtained_.size()) {
        const Entity* entity = character.net_entities_obtained_.front();
        fb::EntityObtainedBuilder entity_obtained_builder(builder);
        entity_obtained_builder.add_id(entity->id());
        entity_obtained_builder.add_type(entity->type());
        auto offset = entity_obtained_builder.Finish();
        entities_obtained_vector.push_back(offset);
        character.net_entities_obtained_.pop();
      }
      auto entities_obtained = builder.CreateVector(entities_obtained_vector);

      fb::Vec2 pos(snapshot.position().x(), snapshot.position().y());
      fb::CharacterBuilder character_builder(builder);
      character_builder.add_pos(&pos);
      character_builder.add_hp(character.hp());
      character_builder.add_bullets(bullets);
      character_builder.add_dash_time(snapshot.dash_time());
      character_builder.add_dash_cooldown(snapshot.dash_cooldown());
      character_builder.add_dash_dir(snapshot.dash_dir());
      character_builder.add_weapon_magazine(snapshot.weapon_magazine());
      character_builder.add_weapon_ammo(snapshot.weapon_ammo());
      character_builder.add_weapon_cooldown(snapshot.weapon_cooldown());
      character_builder.add_weapon_reload_time(snapshot.weapon_reload_time());
      character_builder.add_entities_obtained(entities_obtained);

      auto character_offset = character_builder.Finish();

      fb::PlayerBuilder player_builder(builder);
      player_builder.add_id(player.id());
      player_builder.add_last_input_acked_packet(player.character().last_input_acked_packet_);
      player_builder.add_last_input_remaining_time(player.character().last_input_remaining_time_);
      player_builder.add_character(character_offset);
      auto player_offset = player_builder.Finish();
      players_vector.push_back(player_offset);

      snapshot.Clear();
    }

    auto players = builder.CreateVector(players_vector);
    fb::WorldBuilder world_builder(builder);
    world_builder.add_players(players);
    auto world_offset = world_builder.Finish();

    builder.Finish(world_offset);

    AppendFlatBuffer(builder);
  }

}
