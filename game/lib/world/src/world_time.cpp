// world_time.cpp
#include "world_time.h"

namespace nan2 {

  float Time::delta_time_;
  float Time::current_time_;
  float Time::fixed_delta_time_;
  float Time::current_fixed_time_;

  void Time::delta_time(float dt) {
    Time::delta_time_ = dt;
  }
  float Time::delta_time() {
    return Time::delta_time_;
  }
  void Time::current_time(float t) {
    Time::current_time_ = t;
  }
  float Time::current_time() {
    return Time::current_time_;
  }
  void Time::fixed_delta_time(float t) {
    Time::fixed_delta_time_ = t;
  }
  float Time::fixed_delta_time() {
    return Time::fixed_delta_time_;
  }
  void Time::current_fixed_time(float t) {
    Time::current_fixed_time_ = t;
  }
  float Time::current_fixed_time() {
    return Time::current_fixed_time_;
  }
  
}
