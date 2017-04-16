//
// user.hpp
//

#ifndef _USER_HPP_
#define _USER_HPP_

#include <string>

#include <boost/scoped_ptr.hpp>

#include "../mysql.h"
#include "../redis.h"

namespace nan2::model {
class User {
public:

  User(int user_id, std::string& user_tag)
    : user_id_(user_id)
    , user_tag_(user_tag)
  {
  }

  bool Save(sql::Connection* con)
  {
    // TODO
    return true;
  }

  const int& GetUserId() { return user_id_; }
  const std::string& GetUserTag() { return user_tag_; }
  const short& GetRatingDeath() { return rating_death_; }

  static User* LoadUser(std::string& token,
    cpp_redis::redis_client& redis_client)
  {
    std::vector<std::string> tmp(1);
    tmp[0] = token;
    int *check = new int(1);
    redis_client.exists(tmp, [&, check](cpp_redis::reply &reply) mutable {
      if (reply.as_integer() == 0) *check = 0; 
    });
    redis_client.sync_commit();
    if (*check == 0) return nullptr;

    User* ptr = new User();
    redis_client.hget(token, "user_id",
      [&, ptr](cpp_redis::reply& reply) mutable {
      int idx = 1;
      const std::string& user_id = reply.as_string();
      for (int i = user_id.size() - 1; i >= 0; i--) {
        ptr->user_id_ += (user_id[i] - '0') * idx;
        idx *= 10; 
      }
    });
    redis_client.hget(token, "user_tag",
      [&, ptr](cpp_redis::reply& reply) mutable {
      ptr->user_tag_ = reply.as_string();
    });
    redis_client.hget(token, "rating_death",
      [&, ptr](cpp_redis::reply& reply) mutable {
      short idx = 1;
      const std::string& rating_death = reply.as_string();
      for (int i = rating_death.size() - 1; i >= 0; i--) {
        ptr->rating_death_ += (short)(rating_death[i] - '0') * idx;
        idx *= 10; 
      }
    });
    redis_client.sync_commit();
    return ptr;
  }

  static int CreateUser(std::string& uuid, std::string& user_tag,
    sql::Connection* con)
  {
    int ret;
    boost::scoped_ptr<sql::Statement> statement(con->createStatement());
    boost::scoped_ptr<sql::ResultSet> result;
    std::stringstream sql;

    sql << "INSERT INTO user_infos (user_tag) VALUES ('" << user_tag << "')";
    statement->execute(sql.str());

    // FIXME  don't select and use LAST_INSERTED_ID() with multiple connections
    
    sql.str(std::string());
    sql << "SELECT user_id FROM user_infos WHERE user_tag = '" <<
      user_tag << "'";
    result.reset(statement->executeQuery(sql.str()));
    if (!result->next()) return -1;

    ret = result->getInt("user_id"); 

    sql.str(std::string());
    sql << "INSERT INTO game_records (user_info_key) VALUES (" << ret << ")";
    statement->execute(sql.str());

    sql.str(std::string());
    sql << "INSERT INTO uuid_infos (uuid, user_info_key) VALUES ('" <<
      uuid << "', " << ret << ")";
    statement->execute(sql.str());

    return ret;
  }


private:
  User() { }

  int user_id_;
  int session_id_;
  short rating_death_;
  std::string user_tag_;

  // need to add else..
};
}

#endif

