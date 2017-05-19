// world.cpp
#include "world/world.h"

#include "logger/logger.h"
#include "network/world_snapshot_packet_builder.h"
#include "network/packet_parser.h"
#include "network/player_input_packet_parser.h"
#include "network/pong_packet_parser.h"
#include "network/ping_packet_builder.h"

#include <flatbuffers/flatbuffers.h>
#include "flatbuffers/world_generated.h"

#include <cassert>

namespace nan2 {

  World::World() : 
    last_system_time_(std::chrono::high_resolution_clock::now()),
    snapshot_send_timer_(0),
    ping_send_timer_(0),
    ping_seq_(0),
    last_snapshot_sent_time_(Time::current_time()) {
    world_map_ = new WorldMap();

    // Fill entity id pool
    for (short i = 1; i < 65536; i ++) entity_id_pool_.push(i);

    RootBox* root_box = new RootBox(this, Vector2(30, 30));
    root_boxes_.insert(root_box);
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
    ping_send_timer_ += dt;

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

    if (snapshot_send_timer_ > 100) {
      snapshot_send_timer_ = 0;
      last_snapshot_sent_time_ = Time::current_time();
      TakeSnapshot();
    }
    if (ping_send_timer_ >= 200) {
      ping_send_timer_ = 0;
      SendPingPacket();
    }

  }

  void World::FixedUpdate() {

  }

  short World::AcquireEntityId() {
    if (entity_id_pool_.size() == 0) return 0;
    short ret = entity_id_pool_.front();
    entity_id_pool_.pop();
    return ret;
  }

  void World::ReleaseEntityId(short id) {
    entity_id_pool_.push(id);
  }

  void World::DestroyUpdatables() {
    for (auto item : destroyable_set_) {
      assert(item != nullptr);
      updatable_set_.erase(item);
      updatable_ready_stage_.erase(item);
      item->OnDestroy();
      ReleaseEntityId(item->id());
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
    short id = AcquireEntityId();
    if (id == 0) return false;
    obj->id(id);
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
    players_[id] = player;
    AddUpdatable(&player->character());

    L_DEBUG << "Added Player " << id << " " << players_.count(id);

    return player;
  }

  Player* World::GetPlayer(int id) {
    if (players_.count(id)) return players_[id];
    return nullptr;
  }

  std::map<int, Player*>& World::GetPlayers() {
    return players_;
  }

  bool World::CreateRandomDroppedItemAt(const Vector2& position, DroppedItem*& spawned_item) {
    short id = AcquireEntityId();
    if (id == 0) return false;
    DroppedItem* item = new DroppedItem(this, position);
    dropped_items_.insert(item);
    spawned_item = item;
    return true;
  }

  std::set<RootBox*, entity_comparator>& World::root_boxes() {
    return root_boxes_;
  }

  std::set<DroppedItem*, entity_comparator>& World::dropped_items() {
    return dropped_items_;
  }


  // Netcode

  void World::TakeSnapshot() {
    WorldSnapshotPacketBuilder packet_builder;
    packet_builder.Build(*this);
    OutPacket packet(packet_builder.GetBufferVector(), OutPacket::BROADCAST);
    send_packet_queue_.push(packet);
  }

  void World::OnPacketReceived(boost::shared_ptr<std::vector<char>> buffer, unsigned int& size, uint64_t client_id) {
    std::vector<char>& buffer_vector = *buffer;
    uint8_t* buffer_array = new uint8_t[size];
    std::copy(buffer_vector.begin(), buffer_vector.end(), buffer_array);

    packet_type type = PacketParser::GetPacketType(buffer_array);

    try {
      switch (type) {
        case PacketType::PING:
          break;
        case PacketType::PONG:
          ParsePongPacket(buffer_array, size);
          break;
        case PacketType::SNAPSHOT:
          break;
        case PacketType::PLAYER_INPUT:
          ParsePlayerInputPacket(buffer_array, size, client_id);
          break;
        default:
          break;
      }
    } catch (const char* exp) {
      L_DEBUG << exp;
    }
    delete buffer_array;
  }

  void World::ParsePlayerInputPacket(uint8_t* buffer, unsigned int size, uint64_t client_id) {
    PlayerInputPacketParser parser(buffer, size);
    int player_id;
    auto player_inputs = parser.Parse(player_id);
    Player* player = GetPlayer(player_id);
    cpmap_[player_id] = client_id;
    if (player == nullptr) 
      player = AddPlayer(player_id);
    for (auto input : player_inputs)
      player->character().AddInput(input);
  }

  void World::ParsePongPacket(uint8_t* buffer, unsigned int size) {
    PongPacketParser parser(buffer, size);
    int player_id;
    int seq = parser.Parse(player_id);
    Player* player = GetPlayer(player_id);
    if (player == nullptr) return;
    player->OnPongPacketReceived(seq);
  }

  void World::SendPingPacket() {
    int seq = ping_seq_++;
    for (auto player_item : players_) {
      PingPacketBuilder builder;
      auto player_ = player_item.second;
      builder.Build(seq, player_->ping());
      
      OutPacket packet(builder.GetBufferVector(), OutPacket::UNICAST, cpmap_[player_->id()]);
      player_->PushPingPacket(seq, Time::current_time());
      send_packet_queue_.push(packet);
    }
  }

  unsigned int World::SendPacketQueueSize() const {
    return send_packet_queue_.size();
  }

  const OutPacket World::PopSendPacket() {
    auto ret = send_packet_queue_.front();
    send_packet_queue_.pop();
    return ret;
  }

  int World::last_snapshot_sent_time() const {
    return last_snapshot_sent_time_;
  }
  
}
