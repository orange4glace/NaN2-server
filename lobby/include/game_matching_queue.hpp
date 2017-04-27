//
// matching_queue.hpp
//

#ifndef _MATCHING_QUEUE_
#define _MATCHING_QUEUE_

#include <memory>
#include <iostream>

#include <mgne/pattern/thread_safe_queue.hpp>

#include "group.hpp"
#include "protocol.hpp"

#define MODE_NUM 1

namespace nan2 {
typedef mgne::pattern::ThreadSafeQueue<std::shared_ptr<Group>> GroupQueue;

class InterfaceMQ {
public:
  virtual void PushBronze(std::shared_ptr<Group>) = 0;
  virtual void PushSilver(std::shared_ptr<Group>) = 0;
  virtual void PushGold(std::shared_ptr<Group>) = 0;
  virtual ~InterfaceMQ() = default;
};

template<std::size_t N>
class MatchingQueue : public InterfaceMQ {
public:
  void PushBronze(std::shared_ptr<Group> group) 
  { 
    bronze_[group->GetSize()].Push(group);
  }
  void PushSilver(std::shared_ptr<Group> group)
  {
    silver_[group->GetSize()].Push(group);
  }
  void PushGold(std::shared_ptr<Group> group)
  {
    gold_[group->GetSize()].Push(group);
  }

private:
  std::array<GroupQueue, N> bronze_;
  std::array<GroupQueue, N> silver_;
  std::array<GroupQueue, N>   gold_;
};

class GameMatchingQueue {
public:
  GameMatchingQueue()
  {
    matching_queues_[0].reset(new MatchingQueue<6>());
  }

  bool Push(std::shared_ptr<Group> group_ptr, GameMode mode)
  {
    short rating = -1;
    switch (mode) {
    case GameMode::DEATH: {
      rating = group_ptr->GetDeathRating();
      break;
    }
    }
    
    if (rating == -1) return false;

    if (rating <= 1000) {
      matching_queues_[mode].get()->PushBronze(group_ptr);
    } else if (rating <= 1500) {
      matching_queues_[mode].get()->PushSilver(group_ptr);
    } else {
      matching_queues_[mode].get()->PushGold(group_ptr);
    }
    return true;
  }

  void FindMatch()
  {
    std::cout << "Finding match..!" << std::endl; 
  }
 
public:
  std::array<std::unique_ptr<InterfaceMQ>, 1> matching_queues_;

};
}

#endif
