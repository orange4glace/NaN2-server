//
// matching_queue.hpp
//

#ifndef _MATCHING_QUEUE_
#define _MATCHING_QUEUE_

#include <memory>
#include <unordered_set>

#include <mgne/pattern/thread_job_queue.hpp>

#include "group.hpp"
#include "protocol.hpp"

#define MODE_NUM 1

#define rating_to_tier(x) ((x-1000)/500)

constexpr int sizes[MODE_NUM] = {6};

namespace nan2 {
typedef mgne::pattern::ThreadJobQueue<std::shared_ptr<Group>> GroupQueue;

class InterfaceMQ {
public:
  virtual void Push(int tier, const std::shared_ptr<Group>&) = 0;
  virtual bool Erase(int tier, std::shared_ptr<Group>&) = 0;
  virtual ~InterfaceMQ() = default;
};

template<std::size_t N>
class MatchingQueue : public InterfaceMQ {
public:
  void Push(int tier, const std::shared_ptr<Group>& group)
  {
    queues_[tier][group->GetSize()].Push(group);
  }

  bool Erase(int tier, std::shared_ptr<Group>& group)
  {
    return queues_[tier][group->GetSize()].Erase(group);
  }

private:
  friend class GameMatchingQueue;
  std::array<std::array<GroupQueue, N>, 3> queues_;
};

class GameMatchingQueue {
public:
  GameMatchingQueue()
  {
    matching_queues_[0].reset(new MatchingQueue<6>());
  }

  bool Push(std::shared_ptr<Group>& group_ptr, GameMode mode)
  {
    if (group_set_.find(group_ptr) != group_set_.end()) return false;
    // need to lock

    short rating = -1;

    switch (mode) {
    case GameMode::DEATH: {
      rating = group_ptr->GetDeathRating();
      break;
    }
    }
    
    if (rating == -1) return false;

    matching_queues_[mode]->Push(rating_to_tier(rating), group_ptr);
    group_set_.insert(group_ptr);
    return true;
  }

  bool Erase(std::shared_ptr<Group>& group_ptr, GameMode mode)
  {
    if (group_set_.find(group_ptr) != group_set_.end()) return false;
    // need to lock

    short rating = -1;
    bool ret;
    int tier;

    switch (mode) {
    case GameMode::DEATH: {
      rating = group_ptr->GetDeathRating();
      break;
    }
    }

    if (rating == -1) return false;
    
    if (matching_queues_[mode]->Erase(rating_to_tier(rating), group_ptr))
      return true;

    tier = rating_to_tier(rating);
    mutex_teams_[mode][tier].lock(); // correct way?
    ret = handle_erase(rating_to_tier(rating), group_ptr, mode);
    mutex_teams_[mode][tier].unlock();

    return ret;
  }

  void FindMatch()
  {
    find_match<0, MODE_NUM>();
    std::cout << "Finding match..!" << std::endl; 
  }
 
private:
  template <int st, int ed>
  struct find_match
  { // how about with lambda?
    void operator()()
    {
      if (st < ed)
      {
        std::vector<std::shared_ptr<Group>> trace;
        MatchingQueue<sizes[st]>* mqueue =
          (MatchingQueue<sizes[st]>*)(this->matching_queues_[st].get());

        for (int tier = 0; tier < 3; ++tier) {
          trace.clear();
          this->mutex_teams_[st][tier].lock();
          this->handle_find(this->mqueue->queues_[tier].data(), this->teams_[st][tier],
            0, 6, trace);
          // make game with group set
          this->mutex_teams_[st][tier].unlock();
        }

        find_match<st+1, ed>()();
      }
    }
  };

  template <int n>
  struct find_match<n, n>
  {
    void operator()() { }
  };

  void handle_find(GroupQueue* queues, std::deque<GroupSet>& teams, int size,
    int left, std::vector<std::shared_ptr<Group>>& trace)
  {
    if (left == 0) {
      teams.push_front(GroupSet(trace));
      return;
    }

    if (size == 6) return; // Need to change

    int curr_size = queues[size].Rsize();
    int count = 0;

    for (int idx = curr_size - 1; idx >= 0; idx--) {
      if ((*queues[size].rqueue_)[idx]->GetInGroups()) continue;
      count++;
      (*queues[size].rqueue_)[idx]->Lock();
      trace.push_back((*queues[size].rqueue_)[idx]);
      handle_find(queues, teams, size + 1, left - idx * size, trace);
    }

    while (count--) {
      trace.back()->Unlock();
      trace.pop_back();
    }

    handle_find(queues, teams, size + 1, left, trace);
  }

  bool handle_erase(int tier, std::shared_ptr<Group>& target, GameMode mode)
  {
    GroupSet remain;
    for (auto& group_set : teams_[mode][tier]) {
      if (group_set.FindAndErase(target, remain)) {
        for (auto& group : remain) {
          matching_queues_[mode]->Push(tier, group);
        }
        return true; 
      }
    }
    return false;
  }

  std::array<std::unique_ptr<InterfaceMQ>, MODE_NUM> matching_queues_;

  std::array<std::array<std::deque<GroupSet>, 3>, MODE_NUM> teams_;
  std::array<std::array<std::mutex, 3>, MODE_NUM> mutex_teams_;

  std::unordered_set<std::shared_ptr<Group>> group_set_;
  std::mutex mutex_group_set_;
};
}

#endif
