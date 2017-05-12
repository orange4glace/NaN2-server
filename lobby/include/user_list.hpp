//
// user_list.hpp
//

#ifndef _USER_LIST_HPP_
#define _USER_LIST_HPP_

#include <unordered_set>

#include <mgne/pattern/thread_safe.hpp>

namespace nan2 {
class UserList : mgne::pattern::ThreadSafe {
public:
  void Insert(int id)
  {
    user_list_.insert(id);
  }

  void Delete(int id)
  {
    user_list_.erase(id);
  }

  bool Find(int id)
  {
    return (user_list_.find(id) != user_list_.end());
  }

  void Lock() { ThreadSafe::Lock(); }
  void Unlock() { ThreadSafe::Unlock(); }
private:
  std::unordered_set<int> user_list_;
};
}

#endif
