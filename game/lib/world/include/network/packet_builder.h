#ifndef PACKET_BUILDER_H_
#define PACKET_BUILDER_H_

#include "../flatbuffers/world_generated.h"

#include <flatbuffers/flatbuffers.h>
#include <vector>

#define fb nan2::game::world

namespace nan2 {

  class PacketBuilder {

  private:

  public:

    static uint8_t* BuildPacket(World& world) {

      flatbuffers::FlatBufferBuilder builder(1024);

      fb::WorldBuilder world_builder(builder);
      std::vector<flatbuffers::Offset<fb::Player>> players_vector;
      auto playerMap = world.GetPlayers();

      for (auto it = playerMap.begin(); it != playerMap.end(); it++) {
        Player& player = *it->second;
        fb::PlayerBuilder player_builder(builder);

        Character& character = player.character();
        fb::CharacterBuilder character_builder(builder);
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
        character_builder.add_pos(&pos);
        character_builder.add_hp(100);
        character_builder.add_bullets(bullets);
        character_builder.add_dashing_time(0);
        auto character_offset = character_builder.Finish();

        player_builder.add_id(0);
        player_builder.add_character(character_offset);
        auto player_offset = player_builder.Finish();
        players_vector.push_back(player_offset);
      }

      auto players = builder.CreateVector(players_vector);
      world_builder.add_players(players);
      world_builder.Finish();

      return builder.GetBufferPointer();
    }

  };

}

#endif