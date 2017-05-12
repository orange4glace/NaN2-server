// world.cpp
#include "world/world.h"

#include "logger/logger.h"
#include "network/world_snapshot_packet_builder.h"
#include "network/packet_parser.h"

#include <flatbuffers/flatbuffers.h>
#include "flatbuffers/world_generated.h"

#include <cassert>

namespace nan2 {

  World::World() : 
    last_system_time_(std::chrono::high_resolution_clock::now()),
    snapshot_send_timer_(0),
    last_snapshot_sent_time_(Time::current_time()) {
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
    float u_dt = (float)time_span.count() / 1000.0f;
    Time::delta_time(u_dt);

    int dt = Time::delta_time();
    snapshot_send_timer_ += dt;

    int current_fixed_time = Time::current_time();
    Time::current_time(Time::current_time() + dt);

    StagingUpdatables();

    // Fixed update
    int max_fixed_dt = 33;
    while (dt > 0) {
      int fixed_dt = (dt < max_fixed_dt ? dt : max_fixed_dt);
      Time::fixed_delta_time(fixed_dt);

      FixedUpdate();
      for (Updatable* updatable : updatable_set_) {
        assert(updatable != nullptr);
        if (!updatable->is_on_stage()) continue;
        updatable->FixedUpdate();
      }

      Time::current_fixed_time(current_fixed_time);
      dt -= max_fixed_dt;
    }
    Time::current_fixed_time(Time::current_time());

    for (Updatable* updatable : updatable_set_) {
      assert(updatable != nullptr);
      if (!updatable->is_on_stage()) continue;
      updatable->Update();
    }

    DestroyUpdatables();

    if (snapshot_send_timer_ > 33) {
      snapshot_send_timer_ = 0;
      last_snapshot_sent_time_ = Time::current_time();
      TakeSnapshot();
    }
  }

  void World::FixedUpdate() {

  }

  void World::DestroyUpdatables() {
    for (auto item : destroyable_set_) {
      assert(item != nullptr);
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
      assert(item != nullptr);
      updatable_set_.insert(item);
      // Start updating
      item->set_is_on_stage(true);
      item->OnCreate();
      L_DEBUG << "updatable staged.";
    }
    updatable_ready_stage_.clear();
  }

  bool World::AddUpdatable(Updatable* obj) {
    if (obj == nullptr) return false;
    if (updatable_set_.count(obj)) return false;
    if (updatable_ready_stage_.count(obj)) return false;
    updatable_ready_stage_.insert(obj);
    L_DEBUG << "updatable added.";
    return true;
  }

  void World::DestroyUpdatable(Updatable* obj) {
    if (obj == nullptr) return;
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


  // Netcode

  void World::TakeSnapshot() {
    WorldSnapshotPacketBuilder packet_builder;
    packet_builder.Build(*this);
    send_packet_queue_.push(packet_builder.GetBufferVector());
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

  unsigned int World::SendPacketQueueSize() const {
    return send_packet_queue_.size();
  }

  const std::vector<uint8_t> World::PopSendPacket() {
    auto ret = send_packet_queue_.front();
    send_packet_queue_.pop();
    return ret;
  }

  int World::last_snapshot_sent_time() const {
    return last_snapshot_sent_time_;
  }
  
}