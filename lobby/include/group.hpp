//
// group.hpp
//

#ifndef _GROUP_HPP_
#define _GROUP_HPP_

#include <vector>

#include <mgne/pattern/thread_safe.hpp>

#include "../../include/models/user.hpp"
#include "protocol.hpp"

#define GROUP_MAX_CAPACITY 6

namespace nan2 {
class Group;
class GroupSet;
typedef mgne::pattern::ThreadJobQueue<std::shared_ptr<Group>> GroupQueue;

class Group : mgne::pattern::ThreadSafe {
public:
  Group(int leader)
    : leader_(leader)
    , curr_mode_(GameMode::DEFAULT)
  {
    rating_death_ = 0;
  }

  Group(Group& g1, Group& g2)
    : leader_(g1.leader_)
    , members_(g1.members_.size() + g2.members_.size())
  {
    members_.insert(members_.end(), g1.members_.begin(), g1.members_.end());
    members_.insert(members_.end(), g2.members_.begin(), g2.members_.end());
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

  int GetLeader() { return leader_; }
  int GetSize() { return 1 + members_.size(); }
  bool GetInGroups() { return in_groups_; }
  void SetRatingDeath(short rating) { rating_death_ = rating; }
  short GetRatingDeath() { return rating_death_; }
  void SetCurrMode(GameMode mode) { curr_mode_ = mode; }
  GameMode GetCurrMode() { return curr_mode_; }
  void Lock() { ThreadSafe::Lock(); }
  void Unlock() { ThreadSafe::Unlock(); }
  bool TryLock() { return ThreadSafe::TryLock(); }

private:
  friend class GroupSet;
  bool in_groups_;

  int leader_;
  GameMode curr_mode_;
  short rating_death_;
  std::vector<int> members_;
};

class GroupSet {
public:
  typedef std::unordered_set<std::shared_ptr<Group>>::iterator iterator;

  GroupSet() { }
  GroupSet(std::vector<std::shared_ptr<Group>>& trace)
  {
    for (auto& group : trace) {
      group->in_groups_ = true;
      groups_.insert(group);
    }
  }
  ~GroupSet()
  {
    for (auto& group : groups_) {
      group->in_groups_ = false;
    }
  }

  bool FindAndErase(const std::shared_ptr<Group>& group, GroupSet& remain)
  {
    if (groups_.find(group) != groups_.end()) return false;

    groups_.erase(group);
    for (auto& group: groups_) {
      remain.groups_.insert(group);
      // correct way?
    }
    return true;
  }

  iterator begin() { return groups_.begin(); }
  iterator end() { return groups_.end(); }

private:
  std::unordered_set<std::shared_ptr<Group>> groups_;
  GroupQueue* queues_;

};

class GroupRequests : mgne::pattern::ThreadSafe {
public:
  void Clear()
  {
    in_group_ = false;
    groups_.clear();
  }
  void Erase(Group* group)
  {
    groups_.erase(group);
  }
  void Insert(Group* group)
  {
    groups_.insert(group);
  }
  bool Find(Group* group) 
  {
    return (groups_.find(group) != groups_.end()); 
  }

  void Lock() { ThreadSafe::Lock(); }
  void Unlock() { ThreadSafe::Unlock(); }

  bool in_group_;

private:
  std::unordered_set<Group*> groups_;
};
}

#endif

