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

#define rating_to_tier(x) ((x)/1000)

constexpr int sizes[MODE_NUM] = {6};

namespace nan2 {
typedef mgne::pattern::ThreadJobQueue<std::shared_ptr<Group>> GroupQueue;

class InterfaceMQ {
public:
  virtual void Push(int tier, const std::shared_ptr<Group>&) = 0;
  virtual bool Erase(int tier, const std::shared_ptr<Group>&) = 0;
  virtual ~InterfaceMQ() = default;
};

template<std::size_t N>
class MatchingQueue : public InterfaceMQ {
public:
  void Push(int tier, const std::shared_ptr<Group>& group)
  {
    queues_[tier][group->GetSize()].Push(group);
  }

  bool Erase(int tier, const std::shared_ptr<Group>& group)
  {
    return queues_[tier][group->GetSize()].Erase(group);
  }

private:
  friend class GameMatchingQueue;
  std::array<std::array<GroupQueue, N>, 3> queues_;
};

class GameMatchingQueue {
public:
  GameMatchingQueue(int (*game_handler)(GroupSet&, GroupSet&, GameMode))
    : game_handler_(game_handler)
  {
    matching_queues_[0].reset(new MatchingQueue<6>());
  }

  bool Push(std::shared_ptr<Group>& group_ptr, GameMode mode)
  {
    if (mode == GameMode::DEFAULT) return false;
    std::lock_guard<std::mutex> lock_guard(mutex_group_set_);
    if (group_set_.find(group_ptr) != group_set_.end()) return false;
    // need to lock

    short rating = -1;

    switch (mode) {
    case GameMode::DEATH: {
      rating = group_ptr->GetRatingDeath();
      break;
    }
    default: { }
    }
    
    if (rating == -1) return false;

    matching_queues_[mode]->Push(rating_to_tier(rating), group_ptr);
    group_set_.insert(group_ptr);
    group_sets_[mode].insert(group_ptr);
    return true;
  }

  bool Erase(const std::shared_ptr<Group>& group_ptr, GameMode mode)
  {
    if (mode == GameMode::DEFAULT) return false;

    std::lock_guard<std::mutex> lock_guard(mutex_group_set_);
    std::lock_guard<std::mutex> lock_guard2(mutex_group_sets_[mode]);

    if (group_set_.find(group_ptr) != group_set_.end()) return false;
    if (group_sets_[mode].find(group_ptr) !=
      group_sets_[mode].end()) return false;

    group_sets_[mode].erase(group_ptr);
    group_set_.erase(group_ptr);

    short rating = -1;
    bool ret;
    int tier;

    switch (mode) {
    case GameMode::DEATH: {
      rating = group_ptr->GetRatingDeath();
      break;
    }
    default: { }
    }

    if (rating == -1) return false;

    tier = rating_to_tier(rating);

    ret = (matching_queues_[mode]->Erase(tier, group_ptr));
    if (!group_ptr->GetInGroups()) return ret;
    mutex_teams_[mode][tier].lock();
    ret &= handle_erase(rating_to_tier(rating), group_ptr, mode);
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
          this->handle_find(this->mqueue->queues_[tier].data(),
            this->teams_[st][tier], 0, 6, trace);
          // make game with group set

          int i = this->teams_[st][tier].size() - 1;
          int j = i - 1;
          
          while (j >= 0) {
            int result = this->game_handler_(this->teams_[st][tier][i],
              this->teams_[st][tier][j], st);
            if (result == 0) {
              i = j - 1;
              j = i - 1;
            } else if (result == 1) {
              i = j;
              j = i - 1;
            } else {
              j--;
            }
          }
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

  bool handle_erase(int tier, const std::shared_ptr<Group>& target, GameMode mode)
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

  int (*game_handler_)(GroupSet&, GroupSet&,GameMode);

  std::array<std::unique_ptr<InterfaceMQ>, MODE_NUM> matching_queues_;

  std::array<std::array<std::deque<GroupSet>, 3>, MODE_NUM> teams_;
  std::array<std::array<std::mutex, 3>, MODE_NUM> mutex_teams_;
  std::array<std::unordered_set<std::shared_ptr<Group>>, MODE_NUM> group_sets_;
  std::array<std::mutex, MODE_NUM> mutex_group_sets_;

  std::unordered_set<std::shared_ptr<Group>> group_set_;
  std::mutex mutex_group_set_;
};
}

#endif
