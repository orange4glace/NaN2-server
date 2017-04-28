// packet_parser.cpp
#include "network/packet_builder.h"

#include "logger/logger.h"

namespace nan2 {

  uint8_t* PacketBuilder::BuildPacket(World& world) {

    flatbuffers::FlatBufferBuilder builder(1024);

    std::vector<flatbuffers::Offset<fb::Player>> players_vector;
    auto playerMap = world.GetPlayers();

    for (auto it = playerMap.begin(); it != playerMap.end(); it++) {
      Player& player = *it->second;

      Character& character = player.character();
      CharacterSnapshot& snapshot = character.snapshot();

      std::vector<flatbuffers::Offset<fb::Bullet>> bullets_vector;
      for (const BulletPacket& bullet_packet : snapshot.bullet_packets()) {
        fb::BulletBuilder bullet_builder(builder);
        bullet_builder.add_type(0);
        fb::Vec2 pos(bullet_packet.position().x(), bullet_packet.position().y());
        bullet_builder.add_pos(&pos);
        auto bullet_offset = bullet_builder.Finish();
        bullets_vector.push_back(bullet_offset);
      }
      auto bullets = builder.CreateVector(bullets_vector);

      fb::Vec2 pos(character.position().x(), character.position().y());
      fb::CharacterBuilder character_builder(builder);
      character_builder.add_pos(&pos);
      character_builder.add_hp(100);
      character_builder.add_bullets(bullets);
      character_builder.add_dashing_time(0);
      auto character_offset = character_builder.Finish();

      fb::PlayerBuilder player_builder(builder);
      player_builder.add_id(0);
      player_builder.add_character(character_offset);
      auto player_offset = player_builder.Finish();
      players_vector.push_back(player_offset);
    }

    auto players = builder.CreateVector(players_vector);
    fb::WorldBuilder world_builder(builder);
    world_builder.add_players(players);
    auto world_offset = world_builder.Finish();

    builder.Finish(world_offset);

    return builder.GetBufferPointer();
  }

}