//
// main.cpp for lobby server
//

#include "../include/header.hpp"

#define MAX_CAPACITY       1000
#define MAX_GAME_CAPACITY  1000
#define FIND_MATCH_INTERVAL   5

using namespace nan2;
using namespace nan2::lobby;

std::array<std::unique_ptr<model::User>, MAX_CAPACITY + 1> users;
std::array<std::shared_ptr<Group>, MAX_CAPACITY + 1> groups;
std::array<std::string, MAX_CAPACITY + 1> tokens;
std::array<GroupRequests, MAX_CAPACITY + 1> requests;
std::array<int, MAX_CAPACITY + 1> join_reqs;

int game_handler(GroupSet& A, GroupSet& B, GameMode mode);
int game_count;
GameMode games[MAX_GAME_CAPACITY];
GameMatchingQueue game_matching_queue(game_handler);

UserList user_list;

boost::asio::io_service match_service;
boost::asio::deadline_timer match_timer(match_service);

sql::Driver* driver;
sql::Connection* mysql_con;
cpp_redis::redis_client redis_client;

mgne::tcp::Server* server;

int req_count = 0;

void send(int session_id, short packet_size, short packet_id,
  char* packet_data)
{
  mgne::Packet tmp(packet_size, packet_id, packet_data,
    mgne::Packet::PacketType::PACKET_TCP);
  server->GetSessionManager().Send(session_id, tmp);
}

int game_handler(GroupSet& A, GroupSet& B, GameMode mode)
{
  std::cout << "game_handler()\n";
  bool result_try_lock_A = true;
  bool result_try_lock_B = true;
  int state = -1;

  for (auto& group : A) {
    result_try_lock_A &= group->TryLock();
  }
  for (auto& group : B) {
    result_try_lock_B &= group->TryLock();
  }

  if (result_try_lock_A && result_try_lock_B) {
    // can take both
    state = 0;
  } else if (!result_try_lock_A && result_try_lock_B) {
    state = 1;
  } else if (result_try_lock_A && !result_try_lock_B) {
    state = 2;
  } else {
    // can't take both
    state = 3;
  }
  std::cout << "state : " << state << std::endl;

  if (state == 0) {
    flatbuffers::FlatBufferBuilder builder(1024);
    auto match_ntf = CreateMatchNtf(builder, M_NTF_MATCHED);
    builder.Finish(match_ntf);

    games[game_count] = mode;
    redis_client.set(std::to_string(game_count), std::to_string(mode));
    std::vector<int> sessions; 
    for (auto& group : A) {
      sessions.clear();
      group->GetSessions(sessions);
      for (auto& session : sessions) {
        send(session, builder.GetSize(), PACKET_MATCH_NTF,
          (char*)builder.GetBufferPointer());
        redis_client.hset(tokens[session], "game_num",
          std::to_string(game_count));
      }
    }
    for (auto& group : B) {
      sessions.clear();
      group->GetSessions(sessions);
      for (auto& session : sessions) {
        send(session, builder.GetSize(), PACKET_MATCH_NTF,
          (char*)builder.GetBufferPointer());
        redis_client.hset(tokens[session], "game_num",
          std::to_string(game_count));
      }
    }
    game_count++;
  }

  for (auto& group : A) group->Unlock();
  for (auto& group : B) group->Unlock();
  
  redis_client.sync_commit();
  return state;
}

bool enter_match_queue(std::shared_ptr<Group>& group_ptr, GameMode mode)
{ // need to check mode
  if (group_ptr->GetCurrMode() != GameMode::DEFAULT || 
    mode == GameMode::DEFAULT) return false;
  std::vector<int> sessions;
  short rating = 0;
  group_ptr->GetSessions(sessions);
  group_ptr->SetCurrMode(mode);
  for (auto& id : sessions) {
    rating += users[id]->GetRatingDeath();
  }
  group_ptr->SetRatingDeath(rating / sessions.size());
  return game_matching_queue.Push(group_ptr, mode);
}

bool out_match_queue(std::shared_ptr<Group>& group_ptr)
{
  GameMode curr_mode = group_ptr->GetCurrMode();
  if (curr_mode == GameMode::DEFAULT) return false;
  return game_matching_queue.Erase(group_ptr, curr_mode, false);
}

void match(const boost::system::error_code& error)
{
  game_matching_queue.FindMatch();
  match_timer.expires_from_now(boost::posix_time::seconds(FIND_MATCH_INTERVAL));
  match_timer.async_wait(boost::bind(match, boost::asio::placeholders::error));
}

int find_session_id(std::string& user_tag)
{
  for (int i = 1; i <= MAX_CAPACITY; i++) {
    if (users[i].get() != nullptr &&
      users[i]->GetUserTag().compare(user_tag) == 0) {
      return i;
    }
  }
  return -1;
}

void packet_handler(mgne::Packet& p)
{
  int session_id = p.GetSessionId();
  std::cout << "packet from session : " << session_id << std::endl;

  char* buffer_pointer = p.GetPacketData()->data();
  flatbuffers::FlatBufferBuilder builder(1024);

  switch(p.GetPacketId())
  {
  case PACKET_JOIN_REQ: {
    std::cout << "PACKET_JOIN_REQ\n";
    short state = -1;
    auto join_req = GetJoinReq(buffer_pointer);
    std::string token = join_req->token()->str();
    model::User* tmp = model::User::LoadUser(token, redis_client);

    if (tmp != nullptr) {
      user_list.Lock();
      std::cout << "Enter ready..! " << tmp->GetUserTag() << std::endl;
      std::cout << "Enter id : " << tmp->GetUserId() << std::endl;
      if (user_list.Find(tmp->GetUserId()) == false) {
        state = 1;
        std::cout << "Entered: " << tmp->GetUserTag() << std::endl;
        user_list.Insert(tmp->GetUserId());

        tokens[session_id] = token;
        users[session_id] = std::unique_ptr<model::User>(tmp);
        groups[session_id] = std::make_shared<Group>(session_id);
        requests[session_id].Clear();
      }
      user_list.Unlock();
    } else {
      std::cout << "Enter failed..! " << token << std::endl;
    }
    
    switch (state) {
    case -1: {
      auto join_ans = CreateJoinAns(builder, J_ANS_FAIL_UNAUTH);
      builder.Finish(join_ans);
      break;
    }
    case 1: {
      auto join_ans = CreateJoinAns(builder, J_ANS_SUCC);
      builder.Finish(join_ans);
      break;
    }
    }
    send(p.GetSessionId(), builder.GetSize(), PACKET_JOIN_ANS,
      (char*)builder.GetBufferPointer());
    break;
  }
  case PACKET_GROUP_REQ: {
    std::cout << "PACKET_GROUP_REQ\n";
    model::User* user = users[session_id].get();
    if (user == nullptr) {
      std::cout << "user null ptr\n";
      break; 
    }

    flatbuffers::FlatBufferBuilder builder_ntf(1024);
    flatbuffers::FlatBufferBuilder builder_ntf2(1024);
    short state = -1;
    auto group_req = GetGroupReq(buffer_pointer);
    char req = group_req->req(); 
    switch (req) {
    case G_REQ_JOIN: {
      std::string user_tag = group_req->user_tag()->str();
      std::cout << "Join req to : " << user_tag << std::endl;
      int to_id = find_session_id(user_tag);
      std::cout << "to_id : " << to_id << std::endl; 
      if (to_id != -1) groups[to_id]->Lock();
      requests[session_id].Lock();
      if (to_id != -1 && requests[session_id].in_group_ == false
        && requests[session_id].Find(groups[to_id].get()) == false ) {
        requests[session_id].Insert(groups[to_id].get());
        state = 1;
      }

      if (state == 1) {
        int leader = groups[to_id]->GetLeader();
        auto group_ans = CreateGroupAns(builder, G_ANS_SUCC);
        auto user_tag = builder_ntf.CreateString(user->GetUserTag());
        std::vector<flatbuffers::Offset<flatbuffers::String>> user_tags_(1);
        user_tags_[0] = user_tag;
        auto user_tags = builder_ntf.CreateVector(user_tags_);
        auto group_ntf = CreateGroupNtf(builder_ntf, G_NTF_JOIN_RQ, session_id,
          user_tags);
        builder.Finish(group_ans); 
        builder_ntf.Finish(group_ntf);
      } else {
        auto group_ans = CreateGroupAns(builder, G_ANS_FAIL); 
        builder.Finish(group_ans);
      }
      
      send(session_id, builder.GetSize(), PACKET_GROUP_ANS,
        (char*)builder.GetBufferPointer());
      if (state == 1) {
        send(to_id, builder_ntf.GetSize(), PACKET_GROUP_NTF,
          (char*)builder_ntf.GetBufferPointer());
      }
      requests[session_id].Unlock();
      if (to_id != -1) groups[to_id]->Unlock();
      break;
    }
    case G_REQ_OUT: {
      std::vector<int> sessions;
      if (requests[session_id].in_group_ == true) {
        state = 1;
      }

      if (state == 1) {
        groups[session_id]->Lock();
        groups[session_id]->Out(session_id);
        groups[session_id]->GetSessions(sessions);
        out_match_queue(groups[session_id]);
        groups[session_id]->Unlock();
        groups[session_id] = std::make_shared<Group>(session_id);
        requests[session_id].in_group_ = false;

        auto group_ans = CreateGroupAns(builder, G_ANS_SUCC);
        std::vector<flatbuffers::Offset<flatbuffers::String>> user_tags_(1);
        user_tags_[0] = builder_ntf.CreateString(user->GetUserTag());
        auto user_tags = builder.CreateVector(user_tags_);
        auto group_ntf = CreateGroupNtf(builder_ntf, G_NTF_OUT, 0, user_tags);
        builder.Finish(group_ans);
        builder_ntf.Finish(group_ntf);
      } else {
        auto group_ans = CreateGroupAns(builder, G_ANS_FAIL);
        builder.Finish(group_ans);
      }

      if (state == 1) {
        for (auto id : sessions) {
          send(id, builder_ntf.GetSize(), PACKET_GROUP_NTF,
            (char*)builder_ntf.GetBufferPointer());
        }
      }
      send(session_id, builder.GetSize(), PACKET_GROUP_ANS,
        (char*)builder.GetBufferPointer());
      break;
    }
    case G_REQ_JOIN_AC: {
      std::vector<int> sessions;
      int to_id = group_req->ntf_id();
      std::cout << "to_id : " << to_id << std::endl;
      bool tmp;

      groups[session_id]->Lock();
      requests[to_id].Lock();

      if (requests[to_id].in_group_ == false &&
        requests[to_id].Find(groups[session_id].get()) &&
        groups[session_id]->Join(to_id)) state = 1;
      
      if (state == 1) {
        groups[session_id]->GetSessions(sessions);
        groups[to_id] = groups[session_id];

        auto group_ans = CreateGroupAns(builder, G_ANS_SUCC); 
        std::vector<flatbuffers::Offset<flatbuffers::String>> user_tags_(1);
        user_tags_[0] = builder_ntf.CreateString(users[to_id]->GetUserTag());
        auto user_tags = builder_ntf.CreateVector(user_tags_);
        auto group_ntf = CreateGroupNtf(builder_ntf, G_NTF_JOIN_NEW, 0,
          user_tags);
        user_tags_.clear();
        for (auto id : sessions) 
          user_tags_.push_back(
            builder_ntf2.CreateString(users[id]->GetUserTag())
          );
        auto user_tags_full = builder_ntf2.CreateVector(user_tags_); 
        auto group_ntf2 = CreateGroupNtf(builder_ntf2, G_NTF_JOIN_SC, 0,
          user_tags_full);

        builder.Finish(group_ans);
        builder_ntf.Finish(group_ntf);
        builder_ntf2.Finish(group_ntf2);
      } else {
        auto group_ans = CreateGroupAns(builder, G_ANS_FAIL);
        builder.Finish(group_ans);
      }
      
      if (state == 1) {
        for (auto id : sessions) {
          send(id, builder_ntf.GetSize(), PACKET_GROUP_NTF,
            (char*)builder_ntf.GetBufferPointer());
        }
        send(to_id, builder_ntf2.GetSize(), PACKET_GROUP_NTF,
          (char*)builder_ntf2.GetBufferPointer());
      }
      send(session_id, builder.GetSize(), PACKET_GROUP_ANS,
        (char*)builder.GetBufferPointer());

      requests[to_id].Unlock();
      groups[session_id]->Unlock();
      break;
    }
    case G_REQ_JOIN_DN: {
      int to_id = group_req->ntf_id();  
      groups[session_id]->Lock();
      requests[to_id].Lock();

      if (groups[session_id]->GetLeader() == session_id
        && requests[to_id].Find(groups[session_id].get())) {
        state = 1;
      }
      
      if (state == 1) {
        requests[to_id].Erase(groups[session_id].get());
        auto group_ans = CreateGroupAns(builder, G_ANS_SUCC);
        builder.Finish(group_ans);
      } else {
        auto group_ans = CreateGroupAns(builder, G_ANS_FAIL);
        builder.Finish(group_ans);
      }
      
      send(session_id, builder.GetSize(), PACKET_GROUP_ANS,
        (char*)builder.GetBufferPointer());
      requests[to_id].Unlock();
      groups[session_id]->Unlock();
      break;
    }
    }
    break;
  }
  case PACKET_MATCH_REQ: {
    std::cout << "PACKET_MATCH_REQ\n";
    short state = -1;
    auto match_req = GetMatchReq(buffer_pointer);
    GameMode mode = (GameMode)match_req->mode();
    char req = match_req->req();

    flatbuffers::FlatBufferBuilder builder_ntf(1024);

    switch (req) {
    case M_REQ_JOIN: {
      groups[session_id]->Lock();
      std::cout << "session_id : " << session_id << std::endl;
      if (session_id == groups[session_id]->GetLeader()) {
        state = 1; 
        std::cout << "leader checked\n";
      } else {
        std::cout << "Something's wrong\n";
      }

      if (state == 1 && game_matching_queue.Push(groups[session_id],
        mode) == true) {
        std::cout << "Match req join\n";
        auto match_ans = CreateMatchAns(builder, M_ANS_SUCC); 
        auto match_ntf = CreateMatchNtf(builder_ntf, M_NTF_JOIN);
        builder.Finish(match_ans);
        builder_ntf.Finish(match_ans);
      } else {
        auto match_ans = CreateMatchAns(builder, M_ANS_FAIL);
        builder.Finish(match_ans);
      }

      if (state == 1) {
        std::vector<int> sessions; 
        groups[session_id]->GetSessions(sessions);
        for (auto id : sessions) {
          send(id, builder_ntf.GetSize(), PACKET_MATCH_NTF,
            (char*)builder_ntf.GetBufferPointer());
        }
      }
      send(session_id, builder.GetSize(), PACKET_MATCH_ANS,
        (char*)builder.GetBufferPointer());
      groups[session_id]->Unlock();
      break;
    }
    case M_REQ_OUT: {
      groups[session_id]->Lock();      
      if (session_id == groups[session_id]->GetLeader()) {
        state = 1;
      }

      if (state == 1 &&
        game_matching_queue.Erase(groups[session_id], mode, false) == true) {

        auto match_ans = CreateMatchAns(builder, M_ANS_SUCC);
        auto match_ntf = CreateMatchNtf(builder_ntf, M_NTF_JOIN);
        builder.Finish(match_ans);
        builder_ntf.Finish(match_ans);
      } else {
        auto match_ans = CreateMatchAns(builder, M_ANS_FAIL);
        builder.Finish(match_ans);
      }

      if (state == 1) {
        std::vector<int> sessions;
        groups[session_id]->GetSessions(sessions);
        for (auto id : sessions) {
          send(id, builder_ntf.GetSize(), PACKET_MATCH_NTF,
            (char*)builder_ntf.GetBufferPointer());
        }
      }
      send(session_id, builder.GetSize(), PACKET_MATCH_ANS,
        (char*)builder.GetBufferPointer());
      groups[session_id]->Unlock();
      break;
    }
    }
    break;
  }
  }
}

int main()
{
  FILE *logfile = fopen("lobby.log", "w");
  mgne::log::Logger::SetFile(logfile);

  for (int i = 0; i < MAX_GAME_CAPACITY; i++) games[i] = GameMode::DEFAULT;

  const std::string mysql_url  = MYSQL_URL;
  const std::string mysql_user = MYSQL_USER;
  const std::string mysql_pass = MYSQL_PASS;
  const std::string mysql_db   = MYSQL_DB;

  try {
    driver = sql::mysql::get_driver_instance();
    mysql_con = driver->connect(mysql_url, mysql_user, mysql_pass);
    mysql_con->setSchema(mysql_db);
    std::cout << " - MySQL Connection success" << std::endl;
  } catch (sql::SQLException &e) {
    std::cout << "# ERR: " << e.what();
    std::cout << " (MySQL error code: " << e.getErrorCode();
    std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    return -1;
  }
  
  redis_client.connect(REDIS_URL, REDIS_PORT, [](cpp_redis::redis_client&) {
    std::cout << "# ERR: redis connection lost" << std::endl;
    exit(-1);
  });

  redis_client.sync_commit();
  std::cout << " - Redis Connection success" << std::endl;

  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 4000);
  server = new mgne::tcp::Server(endpoint, MAX_CAPACITY, 3, 3, packet_handler);
  server->RunNonBlock();

  match_timer.expires_from_now(boost::posix_time::seconds(FIND_MATCH_INTERVAL));
  match_timer.async_wait(boost::bind(match, boost::asio::placeholders::error));
  match_service.run();
  return 0;
}
