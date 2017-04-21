//
// group.hpp
//

#ifndef _GROUP_HPP_
#define _GROUP_HPP_

#include <vector>

#include <mgne/pattern/thread_safe.hpp>

#define GROUP_MAX_CAPACITY 6

namespace nan2 {
class Group : mgne::pattern::ThreadSafe {
public:
  Group(int session_id)
    : leader_(session_id)
  {
  }
  ~Group() { }

  bool Join(int session_id)
  {
    if (members_.size() >= GROUP_MAX_CAPACITY - 1) return false;
    members_.push_back(session_id);
    return true;
  }
  
  bool Out(int session_id)
  {
    if (session_id == leader_) {
      leader_ = members_.front(); 
      if (!members_.empty()) members_.erase(members_.begin());
    } else {
      members_.erase(std::remove(members_.begin(), members_.end(), session_id),
        members_.end()); 
    }
    return true;
  }

  void GetSessions(std::vector<int>& sessions)
  {
    sessions.push_back(leader_);
    for (auto id : members_) sessions.push_back(id);
  }

  int GetLeader()
  {
    return leader_;
  }

  int GetSize()
  {
    return 1 + members_.size();
  }

  void Lock() { Lock(); }
  void Unlock() { Unlock(); }

private:
  int leader_;
  std::vector<int> members_;
};
}

#endif

