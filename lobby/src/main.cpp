//
// main.cpp for lobby server
//

#include "../include/header.hpp"

using namespace nan2;
using namespace nan2::lobby;

std::unordered_map<int, std::unique_ptr<model::User>> user_map;
// is unordered_map thread_safe? 

sql::Driver* driver;
sql::Connection* mysql_con;
cpp_redis::redis_client redis_client;
mgne::tcp::Server* server;

void send(int session_id, short packet_size, short packet_id,
  char* packet_data)
{
  mgne::Packet tmp(packet_size, packet_id, packet_data,
    mgne::Packet::PacketType::PACKET_TCP);
  server->GetSessionManager().Send(session_id, tmp);
}

void packet_handler(mgne::Packet& p)
{
  char* buffer_pointer = p.GetPacketData()->data();
  flatbuffers::FlatBufferBuilder builder(1024);

  switch(p.GetPacketId())
  {
  case PACKET_JOIN_REQ: {
    short state = -1;
    auto join_req = GetJoinReq(buffer_pointer);
    std::string token = join_req->token()->str();
    model::User* tmp = model::User::LoadUser(token, redis_client);

    if (tmp != nullptr) {
      state = 1;
      user_map[p.GetSessionId()] = std::unique_ptr<model::User>(tmp);
    }
    // print for debugging
    std::cout << "User joined!\n";
    std::cout << "User id  : " << user_map[p.GetSessionId()]->GetUserId() <<
      std::endl;
    std::cout << "User tag : " << user_map[p.GetSessionId()]->GetUserTag() <<
      std::endl;
    std::cout << "User RD  : " << user_map[p.GetSessionId()]->GetRatingDeath();
    std::cout << std::endl;
    //
    switch (state) {
    case -1: {

      break;
    }
    case 1: {

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
  server = new mgne::tcp::Server(endpoint, 1000, 3, 3, packet_handler);
  server->Run();

  return 0;
}
