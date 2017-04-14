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
  User() { }

  User(int user_id, std::string& uuid, std::string& user_tag) 
    : user_id_(user_id)
    , uuid_(uuid)
    , user_tag_(user_tag)
  {
  }

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

  int GetUserId() { return user_id_; }
  const std::string& GetUUID() { return uuid_; }
  const std::string& GetUserTag() { return user_tag_; }

  static bool CreateUser(std::string& uuid, std::string& user_tag, sql::Connection* con,
    User& user)
  {
    boost::scoped_ptr<sql::Statement> statement(con->createStatement());
    boost::scoped_ptr<sql::ResultSet> result;
    std::stringstream sql;

    sql << "INSERT INTO user_infos (user_tag) VALUES ('" << user_tag << "')";
    statement->execute(sql.str());

    // FIXME  don't select and use LAST_INSERTED_ID() with multiple connections
    
    sql.str(std::string());
    sql << "SELECT user_id FROM user_infos WHERE user_tag = '" << user_tag << "'";
    result.reset(statement->executeQuery(sql.str()));
    if (!result->next()) return false;

    user.user_id_ = result->getInt("user_id"); 
    user.uuid_ = uuid;
    user.user_tag_ = user_tag;

    sql.str(std::string());
    sql << "INSERT INTO game_records (user_info_key) VALUES (" << user.user_id_ << ")";
    statement->execute(sql.str());

    sql.str(std::string());
    sql << "INSERT INTO uuid_infos (uuid, user_info_key) VALUES ('" << user.uuid_ << "', " << user.user_id_ << ")";
    statement->execute(sql.str());

    return true;
  }


private:
  int user_id_;
  int session_id_;
  std::string uuid_;
  std::string user_tag_;

  // need to add else..
};
}

#endif

