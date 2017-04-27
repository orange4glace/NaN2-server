// world_time.h
#ifndef WORLD_TIME_H
#define WORLD_TIME_H

namespace nan2 {

  class Time {

  private:

    Time() {}

    // times will be updated every first of the update by World object.
    static float delta_time_;
    static float current_time_;
    static float fixed_delta_time_;
    static float current_fixed_time_;

  public:

    static inline void delta_time(float dt) {
      Time::delta_time_ = dt;
    }
    static inline float delta_time() {
      return Time::delta_time_;
    }
    static inline void current_time(float t) {
      Time::current_time_ = t;
    }
    static inline float current_time() {
      return Time::current_time_;
    }
    static inline void fixed_delta_time(float t) {
      Time::fixed_delta_time_ = t;
    }
    static inline float fixed_delta_time() {
      return Time::fixed_delta_time_;
    }
    static inline void current_fixed_time(float t) {
      Time::current_fixed_time_ = t;
    }
    static inline float current_fixed_time() {
      return Time::current_fixed_time_;
    }

  };

};

#endif
