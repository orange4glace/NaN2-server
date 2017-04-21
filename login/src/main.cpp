//
// main.cpp for login server
//

#include "../include/header.hpp"

sql::Driver* driver;
sql::Connection* mysql_con;
cpp_redis::redis_client redis_client;
mgne::tcp::Server* server;

thread_local std::random_device rd;
thread_local std::mt19937 gen(rd());
  
using namespace nan2;
using namespace nan2::login;

void gen_random(std::string& s, const int len) {
  static const char chars[] = 
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "!@#$%^&*()_-+=|\\~`";
  s.resize(len);
  std::uniform_int_distribution<int> distribution(0,69);
  for (int i = 0; i < len; i++) {
    s[i] = chars[distribution(gen)];
  }
}

int add_user(std::string& uuid, std::string& user_name,
  std::string& user_tag)
{
  std::uniform_int_distribution<int> distribution(101,4999);

  short num_for_tag;
  short count = 0;

  boost::scoped_ptr<sql::Statement> statement(mysql_con->createStatement());

  while(1) {
    ++count;
    num_for_tag = distribution(gen);
    user_tag = user_name + "#" + std::to_string(num_for_tag);
    std::string query = "SELECT user_id FROM user_infos WHERE user_tag = '" 
      + user_tag + "'";
    boost::scoped_ptr<sql::ResultSet> result(statement->executeQuery(query));
  
    if (!result->rowsCount()) { // no duplication
      return model::User::CreateUser(uuid, user_tag, mysql_con);
    }

    if (count == 100) break;
  }

  return -1; 
}

int login_user(int user_id, std::string& user_tag, std::string& token)
{
  int ret = -1; // failed because of other..
  int* ret_ptr = &ret;
  std::vector<std::string> tmp(1);
  std::vector<std::string> tmp2(1);
  tmp.push_back(std::to_string(user_id));

  redis_client.sadd("logined_user", tmp,
    [&, ret_ptr](cpp_redis::reply& reply) mutable {
    if (reply.as_integer() == 1) *ret_ptr = 1; // success
    else *ret_ptr = -3; // already logined
  });
  redis_client.sync_commit();
  if (ret == 1) {
    int temp = 1;
    int* temp_ptr = &temp;
    int cnt = 0;
    while(temp == 1) {
      if (cnt++ == 100) break;
      gen_random(token, 20);
      tmp2[0] = token;
      redis_client.exists(tmp2,
        [&, temp_ptr](cpp_redis::reply& reply) mutable {
        if (reply.as_integer() == 0) *temp_ptr = 0;
      });
      redis_client.sync_commit();
    }
    if (temp == 1) {
      redis_client.srem("logined_user", tmp);
      redis_client.sync_commit();
      ret = -1;
    } else {
      std::string query = "SELECT rating_death FROM game_records "
        "WHERE user_info_key = " + std::to_string(user_id);
      boost::scoped_ptr<sql::Statement> state(mysql_con->createStatement());
      boost::scoped_ptr<sql::ResultSet> result(state->executeQuery(query));

      if (!result->next()) return -1;

      redis_client.hset(token, "user_id", std::to_string(user_id),
        [&, ret_ptr](cpp_redis::reply& reply) mutable {
        if (reply.as_integer() != 1) *ret_ptr = -1;
      });
      redis_client.hset(token, "user_tag", user_tag,
        [&, ret_ptr](cpp_redis::reply& reply) mutable {
        if (reply.as_integer() != 1) *ret_ptr = -1;
      });
      redis_client.hset(token, "rating_death",
        std::to_string(result->getInt("rating_death")),
        [&, ret_ptr](cpp_redis::reply& reply) mutable {
        if (reply.as_integer() != 1) *ret_ptr = -1;
      });
      redis_client.sync_commit();
    }
  }
  return ret;
}

int get_user_id(std::string& uuid)
{
  std::string query = "SELECT user_info_key FROM uuid_infos WHERE uuid = '"
    + uuid + "'";
  boost::scoped_ptr<sql::Statement> statement(mysql_con->createStatement());
  boost::scoped_ptr<sql::ResultSet> result(statement->executeQuery(query));

  if (result->next()) {
    return result->getInt("user_info_key");
  }
  return -1;
}

int get_user_id_tag(std::string& uuid, std::string& user_tag)
{
  std::string query ="SELECT user_infos.user_id, user_infos.user_tag "
    "FROM user_infos INNER JOIN "
    "uuid_infos ON user_infos.user_id = uuid_infos.user_info_key "
    "WHERE uuid_infos.uuid = '" + uuid + "'" ;
  boost::scoped_ptr<sql::Statement> statement(mysql_con->createStatement());
  boost::scoped_ptr<sql::ResultSet> result(statement->executeQuery(query)); 
  
  if (result->next()) {
    user_tag = result->getString("user_tag");
    return result->getInt("user_id");
  }
  return -1;
}

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
  case PACKET_SIGNUP_REQ: {
    int state = -2;
    auto signup_req = GetSignupReq(buffer_pointer); 
    std::string uuid = signup_req->uuid()->str();
    std::string user_name = signup_req->user_name()->str();
    std::string user_tag = "";

    std::string log = "Signup : " + uuid + ", " + user_name + "\n";
    mgne::log::Logger::Log((char*)log.data());


    if (get_user_id(uuid) == -1) {
      state = add_user(uuid, user_name, user_tag);
    }

    if (state == -1) {
      // FAILED OTHER
      auto user_tag_ = builder.CreateString(user_tag);
      auto signup_ans = CreateSignupAns(builder, S_ANS_FAIL, user_tag_);
      builder.Finish(signup_ans);
    } else if (state == -2) {
      // FAILED REGISTERED
      auto user_tag_ = builder.CreateString(user_tag);
      auto signup_ans = CreateSignupAns(builder, S_ANS_FAIL_REGISTERED,
        user_tag_);
      builder.Finish(signup_ans);
    } else {
      // FAILED SUCCESS
      auto user_tag_ = builder.CreateString(user_tag);
      auto signup_ans = CreateSignupAns(builder, S_ANS_SUCC, user_tag_);
      builder.Finish(signup_ans);
    }

    send(p.GetSessionId(), builder.GetSize(), PACKET_SIGNUP_ANS,
      (char*)builder.GetBufferPointer());
    break;
  }
  case PACKET_LOGIN_REQ: {
    int state = -2;
    auto login_req = GetLoginReq(buffer_pointer);
    std::string uuid = login_req->uuid()->str();
    std::string user_tag;
    std::string token;
    std::string log = "Login : " + uuid + "\n";
    mgne::log::Logger::Log((char*)log.data());

    if ((state = get_user_id_tag(uuid, user_tag)) >= 0) {
      state = login_user(state, user_tag, token);
    } else {
      state = -2;
    }

    if (state == -1) {
      // FAILED OTHER
      auto user_tag_ = builder.CreateString("");
      auto token_ = builder.CreateString("");
      auto login_ans = CreateLoginAns(builder, L_ANS_FAIL,
        user_tag_, token_);
      builder.Finish(login_ans);
    } else if (state == -2) {
      // FAILED UNREGISTERED
      auto user_tag_ = builder.CreateString("");
      auto token_ = builder.CreateString("");
      auto login_ans = CreateLoginAns(builder, L_ANS_FAIL_UNREG,
        user_tag_, token_);
      builder.Finish(login_ans);
    } else if (state == -3) {
      // FAILED LOGINED
      auto user_tag_ = builder.CreateString("");
      auto token_ = builder.CreateString("");
      auto login_ans = CreateLoginAns(builder, L_ANS_FAIL_LOGINED,
        user_tag_, token_);
      builder.Finish(login_ans);
    } else {
      // SUCCESS
      auto user_tag_ = builder.CreateString(user_tag);
      auto token_ = builder.CreateString(token);
      auto login_ans = CreateLoginAns(builder, L_ANS_SUCC,
        user_tag_, token_);
      builder.Finish(login_ans);
    }

    send(p.GetSessionId(), builder.GetSize(), PACKET_LOGIN_ANS,
      (char*)builder.GetBufferPointer());
  }
  }
}

int main()
{
  FILE *logfile = fopen("login.log","w");
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
