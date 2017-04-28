// world.cpp
#include "world/world.h"

#include "logger/logger.h"
#include "network/packet_builder.h"
#include "network/packet_parser.h"

#include <flatbuffers/flatbuffers.h>
#include "flatbuffers/world_generated.h"

namespace nan2 {

  World::World() : 
    last_system_time_(std::chrono::high_resolution_clock::now()),
    packet_send_timer_(0) {
    world_map_ = new WorldMap();
  }

  World::World(WorldMap* world_map) : 
    World() {
    world_map_ = world_map;
  }

  World::~World() {
  }

  const WorldMap* World::world_map() const {
    return world_map_;
  }

  void World::Update() {
    std::chrono::high_resolution_clock::time_point cur_system_time(std::chrono::high_resolution_clock::now());
    std::chrono::duration<double, std::milli> time_span = cur_system_time - last_system_time_;
    last_system_time_ = cur_system_time;
    float dt = (float)time_span.count() / 1000.0f;
    float current_fixed_time = Time::current_time();
    Time::delta_time(dt);
    Time::current_time(Time::current_time() + dt);
    packet_send_timer_ += dt;

    StagingUpdatables();
    
    // For safety
    updatable_set_.erase(nullptr);

    // Fixed update
    float max_fixed_dt = 0.20f;
    while (dt > 0) {
      float fixed_dt = (dt < max_fixed_dt ? dt : max_fixed_dt);
      Time::fixed_delta_time(fixed_dt);
      current_fixed_time += fixed_dt;

      FixedUpdate();
      for (Updatable* updatable : updatable_set_) {
        if (updatable == nullptr) continue;
        if (!updatable->is_on_stage()) continue;
        updatable->FixedUpdate();
      }

      Time::fixed_delta_time(current_fixed_time);
      dt -= max_fixed_dt;
    }
    Time::current_fixed_time(Time::current_time());

    for (Updatable* updatable : updatable_set_) {
      if (updatable == nullptr) continue;
      if (!updatable->is_on_stage()) continue;
      updatable->Update();
    }

    DestroyUpdatables();

    L_DEBUG << packet_send_timer_;
    if (packet_send_timer_ > 1 / 30.0f) {
      packet_send_timer_ = 0;
      TakeSnapshot();
    }
  }

  void World::FixedUpdate() {

  }

  void World::DestroyUpdatables() {
    for (auto item : destroyable_set_) {
      if (item == nullptr) continue;
      updatable_set_.erase(item);
      updatable_ready_stage_.erase(item);
      item->OnDestroy();
      delete item;
    }
    destroyable_set_.clear();
  }

  void World::StagingUpdatables() {
    for (auto item : updatable_ready_stage_) {
      // Already destroied on DestroyUpdatables() or nullptr
      if (item == nullptr) continue;
      updatable_set_.insert(item);
      // Start updating
      item->set_is_on_stage(true);
      item->OnCreate();
    }
    updatable_ready_stage_.clear();
  }

  bool World::AddUpdatable(Updatable* obj) {
    if (updatable_set_.count(obj)) return false;
    if (updatable_ready_stage_.count(obj)) return false;
    updatable_ready_stage_.insert(obj);
    return true;
  }

  void World::DestroyUpdatable(Updatable* obj) {
    destroyable_set_.insert(obj);
    // Stop updating
    obj->set_is_on_stage(false);
  }

  Player* World::AddPlayer(int id) {
    if (players_.count(id)) return players_[id];
    Player* player = new Player(this, id);
    players_.insert({id, player});
    AddUpdatable(&player->character());

    L_DEBUG << "Added Player " << id;

    return player;
  }

  Player* World::GetPlayer(int id) {
    if (players_.count(id)) return players_[id];
    return nullptr;
  }

  std::map<int, Player*>& World::GetPlayers() {
    return players_;
  }

  void World::TakeSnapshot() {
    uint8_t* buffer = PacketBuilder::BuildPacket(*this);
    L_DEBUG << "snapshot";
  }

  void World::OnPacketReceived(uint8_t* buffer) {
    int player_id;
    auto player_inputs = PacketParser::ParsePlayerInputPacket(buffer, player_id);
    Player* player = GetPlayer(player_id);
    if (player == nullptr) 
      player = AddPlayer(player_id);
    for (auto input : player_inputs)
      player->character().AddInput(input);
  }
  
}